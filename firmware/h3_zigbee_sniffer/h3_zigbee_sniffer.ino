/*
 * H3 — Zigbee/Thread Packet Sniffer
 * Capture 802.15.4 IoT traffic using ESP32-C6
 * 
 * Hardware: ESP32-C6
 * 
 * Features:
 *   - Capture IEEE 802.15.4 frames
 *   - Decode Zigbee network layer
 *   - Decode Thread (OpenThread) packets
 *   - Track devices and networks
 *   - Export captured data
 * 
 * WARNING: Educational use only. Respect privacy laws.
 * 
 * Author: 5h4d0wn1k
 * License: MIT
 * Date: 2026-08-26
 */

#include <Arduino.h>
#include "esp_ieee802154.h"
#include "esp_random.h"

// IEEE 802.15.4 Configuration
#define CHANNEL 11  // Default channel (11-26 for 802.15.4)
#define MAX_FRAME_SIZE 127
#define MAX_FRAMES 100

// Frame structure
struct Frame802154 {
    uint8_t data[MAX_FRAME_SIZE];
    uint8_t length;
    int8_t rssi;
    uint32_t timestamp;
    uint8_t channel;
    bool valid;
};

// Device structure
struct IoTDevice {
    uint16_t short_addr;
    uint64_t long_addr;
    uint8_t channel;
    uint32_t last_seen;
    uint32_t frame_count;
    char type[16];  // "Zigbee", "Thread", "Unknown"
};

// Global state
Frame802154 frames[MAX_FRAMES];
int frame_count = 0;
IoTDevice devices[50];
int device_count = 0;
bool capturing = false;

// Zigbee frame types
#define ZIGBEE_FRAME_DATA      0x01
#define ZIGBEE_FRAME_COMMAND   0x03
#define ZIGBEE_FRAME_ACK       0x02

// Thread constants
#define THREAD_MESH_HEADER     0xBC
#define THREAD_DATA_HEADER     0x11

// Function prototypes
void startCapture();
void stopCapture();
void processFrame(uint8_t* data, uint8_t length, int8_t rssi);
void decodeZigbeeHeader(uint8_t* data, uint8_t length);
void decodeThreadHeader(uint8_t* data, uint8_t length);
void listFrames();
void listDevices();
void showFrameDetails(int index);
void exportData();
void showHelp();
void processSerialCommand();

// IEEE 802.15.4 receive callback (weak symbol provided by the 802.15.4 driver;
// the first byte of `frame` is the frame length, data follows it)
extern "C" void IRAM_ATTR esp_ieee802154_receive_done(uint8_t *frame, esp_ieee802154_frame_info_t *frame_info) {
    if (!capturing) {
        esp_ieee802154_receive_handle_done(frame);
        return;
    }

    uint8_t length = frame[0];
    if (length > MAX_FRAME_SIZE) {
        length = MAX_FRAME_SIZE;
    }

    // Store frame
    if (frame_count < MAX_FRAMES) {
        memcpy(frames[frame_count].data, frame + 1, length);
        frames[frame_count].length = length;
        frames[frame_count].rssi = frame_info->rssi;
        frames[frame_count].timestamp = (uint32_t)frame_info->timestamp;
        frames[frame_count].channel = frame_info->channel;
        frames[frame_count].valid = true;

        // Process frame
        processFrame(frame + 1, length, frame_info->rssi);

        frame_count++;
    }

    esp_ieee802154_receive_handle_done(frame);
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== H3 — Zigbee/Thread Sniffer ===");
    Serial.println("IEEE 802.15.4 packet capture");
    Serial.println("WARNING: Educational use only!");
    Serial.println();
    
    // Initialize IEEE 802.15.4
    esp_ieee802154_enable();
    esp_ieee802154_set_channel(CHANNEL);
    esp_ieee802154_set_promiscuous(true);
    esp_ieee802154_receive();
    
    Serial.printf("IEEE 802.15.4 initialized on channel %d\n", CHANNEL);
    Serial.println();
    showHelp();
}

void loop() {
    // Handle serial commands
    if (Serial.available()) {
        processSerialCommand();
    }
}

void processSerialCommand() {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    
    if (cmd == "help") {
        showHelp();
    } else if (cmd == "capture") {
        startCapture();
    } else if (cmd == "stop") {
        stopCapture();
    } else if (cmd == "frames") {
        listFrames();
    } else if (cmd == "devices") {
        listDevices();
    } else if (cmd.startsWith("info ")) {
        int idx = cmd.substring(5).toInt();
        showFrameDetails(idx);
    } else if (cmd == "export") {
        exportData();
    } else if (cmd.startsWith("channel ")) {
        int ch = cmd.substring(8).toInt();
        if (ch >= 11 && ch <= 26) {
            esp_ieee802154_set_channel(ch);
            Serial.printf("Switched to channel %d\n", ch);
        } else {
            Serial.println("Invalid channel! Use 11-26.");
        }
    } else if (cmd == "clear") {
        frame_count = 0;
        device_count = 0;
        Serial.println("Cleared all frames and devices.");
    } else {
        Serial.println("Unknown command. Type 'help' for commands.");
    }
}

void showHelp() {
    Serial.println("\n=== Commands ===");
    Serial.println("capture     - Start capturing 802.15.4 frames");
    Serial.println("stop        - Stop capturing");
    Serial.println("frames      - List captured frames");
    Serial.println("devices     - List discovered devices");
    Serial.println("info N      - Show details for frame N");
    Serial.println("export      - Export captured data as CSV");
    Serial.println("channel N   - Switch to channel N (11-26)");
    Serial.println("clear       - Clear all captures");
    Serial.println("help        - Show this help");
    Serial.println("================\n");
}

void startCapture() {
    capturing = true;
    Serial.println("\n=== Starting Capture ===");
    Serial.printf("Channel: %d\n", CHANNEL);
    Serial.println("Listening for 802.15.4 frames...");
    Serial.println("Press 'stop' to end capture.\n");
}

void stopCapture() {
    capturing = false;
    Serial.printf("\n=== Capture Stopped ===\n");
    Serial.printf("Captured %d frames\n", frame_count);
    Serial.printf("Discovered %d devices\n", device_count);
    Serial.println("======================\n");
}

void processFrame(uint8_t* data, uint8_t length, int8_t rssi) {
    if (length < 1) return;
    
    // Parse frame control
    uint16_t frame_ctrl = (data[1] << 8) | data[0];
    uint8_t frame_type = frame_ctrl & 0x07;
    uint8_t security = (frame_ctrl >> 3) & 0x01;
    uint8_t pan_id_comp = (frame_ctrl >> 6) & 0x01;
    uint8_t dest_addr_mode = (frame_ctrl >> 10) & 0x03;
    uint8_t src_addr_mode = (frame_ctrl >> 14) & 0x03;
    
    // Extract addresses
    uint16_t dest_pan = 0;
    uint64_t dest_addr = 0;
    uint16_t src_pan = 0;
    uint64_t src_addr = 0;
    
    int offset = 2;
    
    // Destination PAN
    if (!pan_id_comp || dest_addr_mode != 0) {
        dest_pan = (data[offset + 1] << 8) | data[offset];
        offset += 2;
    }
    
    // Destination address
    if (dest_addr_mode == 2) {
        dest_addr = (data[offset + 1] << 8) | data[offset];
        offset += 2;
    } else if (dest_addr_mode == 3) {
        dest_addr = 0;
        for (int i = 0; i < 8; i++) {
            dest_addr |= (uint64_t)data[offset + i] << (i * 8);
        }
        offset += 8;
    }
    
    // Source PAN
    if (!pan_id_comp && src_addr_mode != 0) {
        src_pan = (data[offset + 1] << 8) | data[offset];
        offset += 2;
    }
    
    // Source address
    if (src_addr_mode == 2) {
        src_addr = (data[offset + 1] << 8) | data[offset];
        offset += 2;
    } else if (src_addr_mode == 3) {
        src_addr = 0;
        for (int i = 0; i < 8; i++) {
            src_addr |= (uint64_t)data[offset + i] << (i * 8);
        }
        offset += 8;
    }
    
    // Update device list
    bool found = false;
    for (int i = 0; i < device_count; i++) {
        if (devices[i].short_addr == (uint16_t)src_addr) {
            devices[i].last_seen = millis();
            devices[i].frame_count++;
            found = true;
            break;
        }
    }
    
    if (!found && device_count < 50 && src_addr != 0) {
        devices[device_count].short_addr = (uint16_t)src_addr;
        devices[device_count].long_addr = src_addr;
        devices[device_count].channel = CHANNEL;
        devices[device_count].last_seen = millis();
        devices[device_count].frame_count = 1;
        
        // Try to identify protocol
        if (length > offset && data[offset] == THREAD_MESH_HEADER) {
            strcpy(devices[device_count].type, "Thread");
        } else {
            strcpy(devices[device_count].type, "Zigbee");
        }
        
        device_count++;
        
        Serial.printf("\n[NEW DEVICE] 0x%04X (%s)\n", 
                     (uint16_t)src_addr, devices[device_count-1].type);
    }
    
    // Print frame info
    Serial.printf("\n[FRAME] Type:%d RSSI:%d dBm Len:%d\n", 
                 frame_type, rssi, length);
    Serial.printf("  Src: 0x%04X | Dst: 0x%04X | Security: %d\n",
                 (uint16_t)src_addr, (uint16_t)dest_addr, security);
    
    // Decode payload
    if (length > offset) {
        Serial.print("  Payload: ");
        for (int i = offset; i < min((int)length, offset + 16); i++) {
            Serial.printf("%02X ", data[i]);
        }
        Serial.println();
    }
}

void listFrames() {
    if (frame_count == 0) {
        Serial.println("No frames captured yet.");
        return;
    }
    
    Serial.println("\n=== Captured Frames ===");
    for (int i = 0; i < min(frame_count, 20); i++) {
        uint16_t frame_ctrl = (frames[i].data[1] << 8) | frames[i].data[0];
        uint8_t frame_type = frame_ctrl & 0x07;
        
        Serial.printf("[%3d] Type:%d RSSI:%4d Len:%3d | ",
                     i, frame_type, frames[i].rssi, frames[i].length);
        
        // Show first 8 bytes
        for (int j = 0; j < min((int)frames[i].length, 8); j++) {
            Serial.printf("%02X ", frames[i].data[j]);
        }
        if (frames[i].length > 8) Serial.print("...");
        Serial.println();
    }
    Serial.println("=======================\n");
}

void listDevices() {
    if (device_count == 0) {
        Serial.println("No devices discovered yet.");
        return;
    }
    
    Serial.println("\n=== Discovered Devices ===");
    for (int i = 0; i < device_count; i++) {
        uint32_t age = (millis() - devices[i].last_seen) / 1000;
        bool is_recent = (age < 30);
        
        Serial.printf("[%3d] 0x%04X | %-8s | Frames:%5d | %s\n",
                     i, devices[i].short_addr, devices[i].type,
                     devices[i].frame_count,
                     is_recent ? "ACTIVE" : "STALE");
    }
    Serial.println("==========================\n");
}

void showFrameDetails(int index) {
    if (index < 0 || index >= frame_count) {
        Serial.println("Invalid frame index!");
        return;
    }
    
    Frame802154& frame = frames[index];
    
    Serial.printf("\n=== Frame #%d Details ===\n", index);
    Serial.printf("Length: %d bytes\n", frame.length);
    Serial.printf("RSSI: %d dBm\n", frame.rssi);
    Serial.printf("Channel: %d\n", frame.channel);
    Serial.printf("Timestamp: %lu ms\n", frame.timestamp);
    
    Serial.print("Raw Data: ");
    for (int i = 0; i < frame.length; i++) {
        Serial.printf("%02X ", frame.data[i]);
    }
    Serial.println("\n=========================\n");
}

void exportData() {
    if (frame_count == 0) {
        Serial.println("No frames to export.");
        return;
    }
    
    Serial.println("\n=== Exporting CSV Data ===");
    Serial.println("index,type,rssi,length,channel,timestamp,data_hex");
    
    for (int i = 0; i < frame_count; i++) {
        uint16_t frame_ctrl = (frames[i].data[1] << 8) | frames[i].data[0];
        uint8_t frame_type = frame_ctrl & 0x07;
        
        Serial.printf("%d,%d,%d,%d,%d,%lu,",
                     i, frame_type, frames[i].rssi, frames[i].length,
                     frames[i].channel, frames[i].timestamp);
        
        for (int j = 0; j < frames[i].length; j++) {
            Serial.printf("%02X", frames[i].data[j]);
        }
        Serial.println();
    }
    
    Serial.println("==========================\n");
}

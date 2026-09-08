# Zigbee/Thread Packet Sniffer Firmware

## Purpose

Capture IEEE 802.15.4 frames, decode Zigbee/Thread network headers, and track IoT devices.

## Board

- **Board**: ESP32-C6
- **FQBN**: `esp32:esp32:esp32c6`
- **Sketch**: `h3_zigbee_sniffer/h3_zigbee_sniffer.ino`

## Wiring

```
Standalone ESP32-C6 (802.15.4 radio built-in). USB-C for serial/power.
Antenna: onboard PCB antenna. Keep DUT within a few meters.
```

## Build

```bash
arduino-cli compile --fqbn esp32:esp32:esp32c6 firmware/h3_zigbee_sniffer
# upload (example, ESP32-C6):
# arduino-cli upload --fqbn esp32:esp32:esp32c6 --port /dev/ttyACM0 firmware/h3_zigbee_sniffer
```

## Runtime

See the root README "IMPORTANT" section before powering on. This firmware is
for authorized own-lab study. Serial console exposes the interactive command
set described in the root README. All identifiers in the sketch are
placeholders (`lab-*` SSIDs, `00:11:22:33:44:55`, RFC 5737 / example.com).

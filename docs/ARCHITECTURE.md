# H3 — Zigbee/Thread Sniffer Architecture

## System Overview

The H3 Zigbee/Thread Sniffer captures IEEE 802.15.4 frames used by Zigbee and Thread IoT protocols. The ESP32-C6 has built-in 802.15.4 radio support.

## Capture Flow

```
┌─────────────────┐
│  802.15.4 Radio │
│  (ESP32-C6)     │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Frame Parser   │
│  (802.15.4)     │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Protocol Decode│
│  (Zigbee/Thread)│
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Device Track   │
│  (Address/Type) │
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  Data Export    │
│  (CSV/PCAP)     │
└─────────────────┘
```

## IEEE 802.15.4 Frame Structure

### Frame Control Field (2 bytes)

```
Bits 0-2:   Frame type
  000 = Beacon
  001 = Data
  010 = Acknowledgment
  011 = MAC command

Bits 3:     Security enabled
Bit 4:      Frame pending
Bit 5:      AR (ack request)
Bits 6-7:   PAN ID compression
Bits 8-9:   Reserved
Bits 10-11: Destination address mode
  00 = No address
  01 = Reserved
  10 = 16-bit short
  11 = 64-bit extended
Bits 12-13: Frame version
Bits 14-15: Source address mode
```

### Addressing Modes

| Mode | Size | Description |
|------|------|-------------|
| 00 | 0 bytes | No address |
| 01 | 2 bytes | Reserved |
| 10 | 2 bytes | 16-bit short address |
| 11 | 8 bytes | 64-bit extended address |

## Zigbee Network Layer

### Frame Types

| Type | Value | Description |
|------|-------|-------------|
| Data | 0x01 | Data frame |
| Command | 0x03 | Network command |
| ACK | 0x02 | Acknowledgment |

### Common Commands

| Command ID | Name |
|------------|------|
| 0x01 | Network Discovery |
| 0x02 | Router Information |
| 0x03 | Device Announcement |
| 0x04 | Data Request |
| 0x05 | MAC Address Conflict |

## Thread Protocol

### Mesh Header

```
Byte 0: 0xBC (Mesh header identifier)
Byte 1: Header control
  Bit 0: V (Vantage)
  Bits 1-2: Fabric ID
  Bits 3-7: Sequence number
```

### Data Header

```
Byte 0: 0x11 (Data header identifier)
Byte 1: Header control
  Bits 0-1: Message type
  Bits 2-3: Delivery mode
  Bit 4: Security enabled
```

## Device Identification

### By Source Address

- **Short Address (16-bit)**: Assigned by network coordinator
- **Extended Address (64-bit)**: Factory-assigned IEEE address

### By Frame Content

| Indicator | Protocol |
|-----------|----------|
| 0xBC mesh header | Thread |
| Zigbee cluster IDs | Zigbee |
| Manufacturer-specific | Vendor-proprietary |

## Common IoT Devices

### Zigbee Devices

- Philips Hue lights
- SmartThings sensors
- IKEA TRADFIR
- Xiaomi/Aqara sensors
- GE/Jasco switches

### Thread Devices

- Google Nest Hub
- Apple HomePod
- Amazon Echo (4th gen)
- Nordic Thingy:91

## Capture Applications

### Network Mapping

1. Discover all devices on Zigbee/Thread network
2. Map device relationships (parent-child)
3. Identify network topology

### Traffic Analysis

1. Monitor device communication patterns
2. Detect unusual traffic (potential attacks)
3. Analyze protocol usage

### Security Assessment

1. Test for unencrypted traffic
2. Verify security configurations
3. Identify vulnerable devices

## Defensive Countermeasures

### For IoT Devices

1. **Enable encryption**: Use AES-128-CCM
2. **Join codes**: Require authorized joining
3. **Network keys**: Rotate periodically
4. **Device authentication**: Verify device identity

### For Network Administrators

1. **Network segmentation**: Isolate IoT traffic
2. **Monitoring**: Deploy 802.15.4 IDS
3. **Physical security**: Control device access
4. **Firmware updates**: Keep devices patched

## Integration with Other Projects

```
H3 (Zigbee Sniffer) ──feeds──▶ I4 (Smart Home Attack)
     │
     ▼
I5 (SCADA Lab) ◀──extends── H3
     │
     ▼
H15 (Spectrum War) ──includes── 802.15.4 analysis
```

## References

- IEEE 802.15.4-2020 Specification
- Zigbee Cluster Library
- Thread Protocol Specification
- ESP32-C6 IEEE 802.15.4 Driver

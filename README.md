# H3 — Zigbee/Thread Packet Sniffer

Capture and analyze IEEE 802.15.4 IoT traffic using ESP32-C6.

## Overview

This project implements an IEEE 802.15.4 packet sniffer that:
- Captures Zigbee and Thread network traffic
- Decodes frame headers and addresses
- Tracks IoT devices on the network
- Exports captured data for analysis

**WARNING: Educational use only. Respect privacy laws.**

## Hardware

| Component | Connection | Role |
|-----------|------------|------|
| ESP32-C6 | Main board | 802.15.4 radio |

## Serial Commands

```
capture     - Start capturing 802.15.4 frames
stop        - Stop capturing
frames      - List captured frames
devices     - List discovered devices
info N      - Show details for frame N
export      - Export captured data as CSV
channel N   - Switch to channel N (11-26)
clear       - Clear all captures
help        - Show commands
```

## IEEE 802.15.4 Channels

| Channel | Frequency | Use Case |
|---------|-----------|----------|
| 11 | 2405 MHz | Zigbee/Thread |
| 12 | 2410 MHz | Zigbee/Thread |
| ... | ... | ... |
| 26 | 2480 MHz | Zigbee/Thread |

## Example Session

```
=== H3 — Zigbee/Thread Sniffer ===
IEEE 802.15.4 initialized on channel 11

[NEW DEVICE] 0x1234 (Zigbee)
[FRAME] Type:1 RSSI:-45 dBm Len:42
  Src: 0x1234 | Dst: 0x0000 | Security: 1
  Payload: 01 03 00 12 34 00 00 ...

[NEW DEVICE] 0x5678 (Thread)
[FRAME] Type:1 RSSI:-62 dBm Len:28
  Src: 0x5678 | Dst: 0x0000 | Security: 1
  Payload: BC 11 00 56 78 ...
```

## Build & Flash

```bash
arduino-cli compile --fqbn esp32:esp32:esp32c6 h3_zigbee_sniffer
arduino-cli upload --fqbn esp32:esp32:esp32c6 --port /dev/ttyACM0 h3_zigbee_sniffer
```

## Research Value

- **H3 — Zigbee Sniffer**: This project
- **I4 — Smart Home Attack Suite**: Target Zigbee/Thread devices
- **I5 — SCADA Lab**: Extend to industrial protocols
- **H15 — Spectrum Warfare**: Multi-protocol interference analysis

## References

- IEEE 802.15.4 Specification
- Zigbee Network Layer
- Thread Protocol
- ESP32-C6 802.15.4 Documentation

## License

MIT

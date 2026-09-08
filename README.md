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

## Live Lab Test Plan

Run ONLY on an isolated, authorized own-lab bench against devices, networks,
and spectrum **you own**. No third-party callers, bystanders, or spectrum users
may be within range of any test transmission.

1. **Isolate** - Put the DUT in a shielded/Faraday enclosure or a room with no
   third-party devices in range. Use attenuators on any transmit path.
2. **Own devices only** - Every target (AP, remote, tag, GPS module, drone FC,
   receiver) must be your own hardware.
3. **Lowest power, shortest duration** - Start at minimum TX power / duty cycle
   and use only the seconds needed.
4. **Record** - Save before/after logs to `reports/` (git-ignored). Never
   capture or store third-party traffic.
5. **Cleanup** - Restore placeholder SSIDs (`lab-*`), MACs (`00:11:22:33:44:55`),
   example.com / RFC5737 addresses, and clear any captured data from the device.

> Jammer / spoofer / replay projects are **proofs for study and simulation**
> only. They refuse live interference scenarios: a live bench trigger requires
> the `LAB_*` allowlist environment variable AND explicit `--yes` confirmation,
> and even then only against your own hardware in a shielded bench.

## Metrics

| Metric | Target | Where |
|---|---|---|
| Firmware compile | `arduino-cli compile --fqbn esp32:esp32:esp32c6 firmware/h3_zigbee_sniffer` PASS | CI/local |
| Host helper | `python3 host/h3_cli.py --demo` exits 0 (offline) | host/ |
| Unit tests | `python3 -m unittest discover -s tests` passes | tests/ |
| py_compile | every `host/*.py` compiles clean | CI/local |

## License

MIT

## Legal Disclaimer

## IMPORTANT: Read before use.

This project is provided for **educational and authorized security testing purposes only**. 

### Authorization Requirements
- You MUST have explicit written permission from the network owner before using this tool
- Unauthorized interception of network communications is illegal under federal and state laws
- This tool should ONLY be used on networks you own or have written authorization to test

### Legal Framework
- **Computer Fraud and Abuse Act (CFAA)**: Unauthorized access to computer systems is a federal crime
- **Wiretap Act (18 U.S.C. § 2511)**: Interception of electronic communications without consent is illegal
- **State Laws**: Many states have additional computer crime and wiretapping statutes
- **GDPR/CCPA**: Data collection may be subject to privacy regulations

### Acceptable Use
- Testing security of your own networks
- Authorized penetration testing with written scope
- Academic research in controlled lab environments
- Security education and training

### Prohibited Use
- Intercepting communications on networks you do not own
- Attacking infrastructure without authorization
- Any activity that violates applicable laws or regulations
- Commercial use without proper licensing

### No Warranty
This software is provided "AS IS" without warranty of any kind. The author is not responsible for any misuse or damage caused by this software.

### Responsible Disclosure
If you discover vulnerabilities using this tool, follow responsible disclosure practices:
1. Report to the vendor/owner privately
2. Allow reasonable time for remediation
3. Do not exploit beyond proof of concept

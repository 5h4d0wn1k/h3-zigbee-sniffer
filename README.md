> **⚠️ EDUCATIONAL USE ONLY — AUTHORIZED TESTING ONLY.**
> This project exists for education, research, and **defense of systems you own
> or hold explicit written authorization to assess**. Unauthorized use is
> prohibited and may be illegal. Read [ETHICS.md](ETHICS.md) and
> [SCOPE.md](SCOPE.md) before use. Use at your own risk; **AS IS**, no warranty.

# H3 — Zigbee/Thread Packet Sniffer

![License: MIT](https://img.shields.io/badge/license-MIT-blue.svg)
![GitHub Stars](https://img.shields.io/github/stars/5h4d0wn1k/h3-zigbee-sniffer)
![Last Commit](https://img.shields.io/github/last-commit/5h4d0wn1k/h3-zigbee-sniffer)
![GitHub Issues](https://img.shields.io/github/issues/5h4d0wn1k/h3-zigbee-sniffer)

> **IEEE 802.15.4 packet sniffer for ESP32-C6** — capture and decode Zigbee and
> Thread frames, track devices, switch channels, and export captures as CSV for
> IoT security research in controlled labs.

## Why

Zigbee and Thread form the backbone of smart-home and industrial IoT, yet their
802.15.4 frames are rarely visible to mainstream tooling. H3 makes that traffic
observable: an ESP32-C6 radio captures 802.15.4 frames across channels 11–26
(2405–2480 MHz), the firmware decodes frame headers, addresses, and security
flags, tracks the devices that appear, and exports the session as CSV — while a
Python host helper (`host/h3_cli.py`) analyzes hex frame dumps fully offline.
The goal is hands-on IoT traffic analysis education: watch your own network's
addressing, identify neighbors in range, and learn how encrypted, secure-coordinator
profiles differ from open ones — all within an isolated, authorized lab.

## Features

- **802.15.4 frame capture** — channel-sniff Zigbee/Thread frames on ESP32-C6.
- **Channel control** — switch channels 11–26 over serial (`channel N`).
- **Frame decode** — headers, source/destination addresses, security flag, RSSI.
- **Device tracking** — serial console discovery list.
- **CSV export** — dump captured frames for offline analysis.
- **Offline host helper** — `python3 host/h3_cli.py --demo` parses bundled hex
  fixtures (exit 0, no radio needed).
- **Fixtures** — `fixtures/frames.hex` for deterministic testing.

## Quickstart

```bash
# Offline analysis (no radio required)
python3 host/h3_cli.py --demo
python3 host/h3_cli.py --file fixtures/frames.hex

# Unit tests
python3 -m unittest discover -s tests
```

## ESP32-C6 firmware

```bash
arduino-cli compile --fqbn esp32:esp32:esp32c6 firmware/h3_zigbee_sniffer
arduino-cli upload --fqbn esp32:esp32:esp32c6 --port /dev/ttyACM0 firmware/h3_zigbee_sniffer
```

Serial commands: `capture`, `stop`, `frames`, `devices`, `info N`, `export`,
`channel N`, `clear`, `help`.

## Project structure

```
firmware/h3_zigbee_sniffer/   # ESP32-C6 Arduino firmware
host/                         # Python host helpers (h3_cli.py, hw_common.py)
fixtures/                     # offline hex frame dumps
docs/                         # ARCHITECTURE.md
tests/                        # offline unit tests
```

## Documentation

- [docs/ARCHITECTURE.md](docs/ARCHITECTURE.md) — design notes
- [ETHICS.md](ETHICS.md) — ethical-use policy, read first
- [SCOPE.md](SCOPE.md) — authorized-scope definition
- [CONTRIBUTING.md](CONTRIBUTING.md) — how to contribute
- [SECURITY.md](SECURITY.md) — vulnerability reporting

## Contributing

New frame decoders, export formats, and fixture generators are welcome. See
[CONTRIBUTING.md](CONTRIBUTING.md).

## License

MIT — see [LICENSE](LICENSE).
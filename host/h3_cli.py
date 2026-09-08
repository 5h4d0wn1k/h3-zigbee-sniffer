#!/usr/bin/env python3
"""H3 - Zigbee/Thread sniffer host helper: offline 802.15.4 frame decode.
Educational/authorized own-lab use only (see README "IMPORTANT").
"""
import argparse
import os
import sys

MOD = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, MOD)
from hw_common import DEMO_TAG, read_target


def decode(bytes_):
    if len(bytes_) < 2:
        return None
    fctrl = (bytes_[1] << 8) | bytes_[0]
    ftype = fctrl & 0x07
    src = None
    dst = None
    off = 2
    dest_addr_mode = (fctrl >> 10) & 0x3
    src_addr_mode = (fctrl >> 14) & 0x3
    pan_id_comp = (fctrl >> 6) & 0x1
    if ftype == 0:
        return None  # beacon
    if not pan_id_comp or dest_addr_mode != 0:
        off += 2
    if dest_addr_mode == 2:
        dst = (bytes_[off + 1] << 8) | bytes_[off]; off += 2
    elif dest_addr_mode == 3:
        dst = int.from_bytes(bytes_[off:off + 8], "little"); off += 8
    if not pan_id_comp and src_addr_mode != 0:
        off += 2
    if src_addr_mode == 2:
        src = (bytes_[off + 1] << 8) | bytes_[off]
    elif src_addr_mode == 3:
        src = int.from_bytes(bytes_[off:off + 8], "little")
    proto = "Thread" if (len(bytes_) > off and bytes_[off] == 0xBC) else "Zigbee"
    return {"type": ftype, "src": src, "dst": dst, "proto": proto, "len": len(bytes_)}


def analyze(text):
    frames = []
    for line in text.splitlines():
        line = line.strip()
        if not line or line.startswith("#"):
            continue
        try:
            b = bytes.fromhex(line)
        except ValueError:
            continue
        d = decode(b)
        if d:
            d["raw"] = line
            frames.append(d)
    return frames


def run_demo():
    print("=== H3 802.15.4 decode (offline) ===")
    text = read_target(
        "fixtures/frames.hex",
        "61 88 01 00 02 00 00 00 00 ff ff 01 00 01 02 03\n"
        "41 cc 06 00 80 0000 0000 0000 11 22 33 44\n")
    for f in analyze(text):
        print("  type=%d src=%s dst=%s [%s] len=%d"
              % (f["type"], f["src"], f["dst"], f["proto"], f["len"]))
    print(DEMO_TAG)
    return 0


def main(argv=None):
    p = argparse.ArgumentParser(
        description="H3 Zigbee/Thread sniffer - offline frame decode")
    p.add_argument("--demo", action="store_true", help="offline demo (exit 0)")
    p.add_argument("--file", help="hex frame dump path")
    args = p.parse_args(argv)
    text = read_target("fixtures/frames.hex")
    if args.file:
        text = open(args.file).read()
    if args.demo or args.file:
        for f in analyze(text):
            print("type=%d src=%s dst=%s %s len=%d" %
                  (f["type"], f["src"], f["dst"], f["proto"], f["len"]))
        return 0
    return run_demo()


if __name__ == "__main__":
    sys.exit(main())

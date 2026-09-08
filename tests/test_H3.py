import os
import sys
import unittest

sys.path.insert(0, os.path.join(os.path.dirname(__file__), "..", "host"))
import h3_cli as m


class TestDecode(unittest.TestCase):
    def test_decode_zigbee_data(self):
        raw = bytes.fromhex("61 88 01 00 02 00 00 00 00 ff ff 01 00 01 02 03")
        d = m.decode(raw)
        self.assertIsNotNone(d)
        self.assertEqual(d["type"], 1)  # data frame
        self.assertEqual(d["proto"], "Zigbee")
        self.assertEqual(d["len"], len(raw))

    def test_decode_thread(self):
        raw = bytes.fromhex("41 cc 06 00 80 00 00 00 00 00 00 11 22 33 44")
        d = m.decode(raw)
        self.assertIsNotNone(d)

    def test_empty(self):
        self.assertIsNone(m.decode(b"\x00"))


if __name__ == "__main__":
    unittest.main()

#!/usr/bin/env python3
"""Patch the verified private base loader for the tested SDL button mapping."""
from hashlib import sha256
from pathlib import Path
from struct import pack_into, unpack_from

root = Path(__file__).resolve().parents[3]
components = root / ".local" / "components"
base = components / "crossy-loader-unitybase"
output = components / "crossy-loader-sdl-single"
expected_base = "2abeead5ead2b7bd5fe4ce8e2cd302c954b43e3dad3e429828ae25ee93d32176"
expected_output = "0e611578009776ec0d5f2d309fe333dd3ccc1b81d45bbb179c41e74e6cbcb7b7"

data = bytearray(base.read_bytes())
assert sha256(data).hexdigest() == expected_base, "Pinned loader changed"
offset = 0x416AF8 - 0x400000
old = 0x52800000 | (108 << 5)
new = 0x52800000 | (82 << 5)
assert unpack_from("<I", data, offset)[0] == old, "Start opcode changed"
pack_into("<I", data, offset, new)
assert sha256(data).hexdigest() == expected_output, "Unexpected patched loader"
output.write_bytes(data)
output.chmod(0o755)
print(f"Wrote {output} ({expected_output})")

#!/usr/bin/env python3
"""Validate the distinct catalogue cover and PortMaster gameplay screenshot."""
import hashlib
from pathlib import Path
import struct
import sys


def dimensions(path):
    data = path.read_bytes()
    if len(data) < 33 or data[:8] != b"\x89PNG\r\n\x1a\n" or data[12:16] != b"IHDR":
        raise ValueError(f"{path.name}: expected PNG")
    width, height = struct.unpack(">II", data[16:24])
    if not width or not height:
        raise ValueError(f"{path.name}: invalid dimensions")
    return width, height, hashlib.sha256(data).digest()


def main():
    directory = Path(sys.argv[1])
    width, height, cover_hash = dimensions(directory / "cover.png")
    if height < width * 1.25:
        raise ValueError("cover.png: expected vertical 2D box art, preferably 2:3; a gameplay screenshot is not a cover")
    sw, sh, screenshot_hash = dimensions(directory / "screenshot.png")
    if sw < 640 or sh < 480 or sw * 3 != sh * 4:
        raise ValueError("screenshot.png: expected 4:3 gameplay, at least 640x480")
    if cover_hash == screenshot_hash:
        raise ValueError("cover.png and screenshot.png must be different media")
    print(f"Media validated: cover {width}x{height}; screenshot {sw}x{sh}")


if __name__ == "__main__":
    try:
        main()
    except (ValueError, OSError, IndexError) as error:
        sys.exit(f"Media validation failed: {error}")

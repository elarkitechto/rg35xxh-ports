#!/usr/bin/env python3
"""Read-only inventory and identity check for the pinned Threes! full 3.1.6144 APK."""
from __future__ import annotations

import hashlib
import sys
import zipfile
from pathlib import Path

PINNED_SHA256 = "7d28059f0993285382da108324f844d146b375b64a1600fac4b8e6f5cf3c482a"
REQUIRED = (
    "assets/bin/Data/globalgamemanagers",
    "assets/bin/Data/Managed/Metadata/global-metadata.dat",
    "lib/arm64-v8a/libil2cpp.so",
    "lib/arm64-v8a/libunity.so",
)


def main() -> int:
    if len(sys.argv) != 2:
        print(f"usage: {Path(sys.argv[0]).name} APK", file=sys.stderr)
        return 2
    apk = Path(sys.argv[1])
    if not apk.is_file():
        print(f"APK not found: {apk}", file=sys.stderr)
        return 2

    digest = hashlib.sha256()
    with apk.open("rb") as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(block)
    actual = digest.hexdigest()
    print(f"APK: {apk}")
    print(f"SHA-256: {actual}")
    print(f"Pinned candidate: {'yes' if actual == PINNED_SHA256 else 'NO'}")

    try:
        with zipfile.ZipFile(apk) as archive:
            members = {item.filename: item for item in archive.infolist()}
            abis = sorted({p.split("/")[1] for p in members if p.startswith("lib/") and len(p.split("/")) > 2})
            print(f"Native ABIs: {', '.join(abis) if abis else '(none found)'}")
            missing = [name for name in REQUIRED if name not in members or members[name].file_size == 0]
            for name in REQUIRED:
                item = members.get(name)
                print(f"{'OK' if item and item.file_size else 'MISSING'} {name}" + (f" ({item.file_size} bytes)" if item else ""))
            for name in ("lib/arm64-v8a/libil2cpp.so", "lib/arm64-v8a/libunity.so", "assets/bin/Data/Managed/Metadata/global-metadata.dat"):
                if name in members:
                    print(f"  {name} SHA-256: {hashlib.sha256(archive.read(name)).hexdigest()}")
    except zipfile.BadZipFile:
        print("Invalid APK/ZIP archive", file=sys.stderr)
        return 2

    if actual != PINNED_SHA256 or missing or "arm64-v8a" not in abis:
        print("Candidate check failed; do not use this APK for a port build.", file=sys.stderr)
        return 1
    print("Candidate identity and required ARM64 Unity files match.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())

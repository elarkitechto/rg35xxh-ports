#!/usr/bin/env python3
"""Read-only audit for the pinned Threes! Free 3.1 ARM64 XAPK."""
from __future__ import annotations
import hashlib, io, re, sys, zipfile
from pathlib import Path

PINNED_XAPK = "99dcba5f8dbcbc4294507c6b30d9f0719ae4127228186e9a471ecdb45a77c880"
PINNED_BASE = "51f8f11f9a8e11ca74ff00edd795509c59fc7b48d8dedd291300b57b0ae935c7"
PINNED_SPLIT = "06804676eace4fe6d51236968fa57a7192ef0e00a9f651fdbbe3687a5ad21f61"
LIBS = ("lib/arm64-v8a/libil2cpp.so", "lib/arm64-v8a/libunity.so", "lib/arm64-v8a/libmain.so")
ASSETS = ("assets/bin/Data/globalgamemanagers", "assets/bin/Data/Managed/Metadata/global-metadata.dat")

def sha(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()

def main() -> int:
    if len(sys.argv) != 2:
        print(f"usage: {Path(sys.argv[0]).name} GAME.xapk", file=sys.stderr)
        return 2
    path = Path(sys.argv[1])
    if not path.is_file():
        print(f"file not found: {path}", file=sys.stderr)
        return 2
    raw = path.read_bytes()
    digest = sha(raw)
    print(f"XAPK SHA-256: {digest} ({'pinned' if digest == PINNED_XAPK else 'not pinned'})")
    try:
        with zipfile.ZipFile(io.BytesIO(raw)) as outer:
            names = set(outer.namelist())
            if not {"vo.threes.free.apk", "config.arm64_v8a.apk"}.issubset(names):
                raise ValueError("expected base and config.arm64_v8a splits")
            base = outer.read("vo.threes.free.apk")
            split = outer.read("config.arm64_v8a.apk")
            print(f"base APK SHA-256: {sha(base)}")
            print(f"ARM64 split SHA-256: {sha(split)}")
            with zipfile.ZipFile(io.BytesIO(base)) as b, zipfile.ZipFile(io.BytesIO(split)) as a:
                gm = b.read("assets/bin/Data/globalgamemanagers")
                match = re.search(rb"20[0-9]{2}\.[0-9]+\.[0-9]+f[0-9]+", gm)
                print(f"Unity version: {match.group().decode() if match else 'not identified'}")
                bad = []
                for name in ASSETS:
                    ok = name in b.namelist() and bool(b.getinfo(name).file_size)
                    print(f"{'OK' if ok else 'MISSING'} base/{name}")
                    if not ok:
                        bad.append(name)
                for name in LIBS:
                    ok = name in a.namelist() and bool(a.getinfo(name).file_size)
                    if ok:
                        content = a.read(name)
                        print(f"OK ARM64/{name}: {len(content)} bytes SHA-256={sha(content)}")
                    else:
                        print(f"MISSING ARM64/{name}")
                        bad.append(name)
    except (zipfile.BadZipFile, KeyError, ValueError) as exc:
        print(f"invalid XAPK: {exc}", file=sys.stderr)
        return 2
    if digest != PINNED_XAPK or sha(base) != PINNED_BASE or sha(split) != PINNED_SPLIT or bad:
        print("Pinned Threes! 3.1 ARM64 candidate check failed.", file=sys.stderr)
        return 1
    print("Pinned candidate and required Unity/IL2CPP ARM64 files match.")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())

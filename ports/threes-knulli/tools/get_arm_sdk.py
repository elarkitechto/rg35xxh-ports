"""Fetch only SDL2 headers and the ARM64 link reference, with pinned hashes."""
import argparse
import hashlib
import io
from pathlib import Path
import tarfile
import urllib.request

parser = argparse.ArgumentParser()
parser.add_argument('destination', type=Path)
args = parser.parse_args()
packages = [
    ('libsdl2-2.0-0_2.0.14+dfsg2-3+deb11u1_arm64.deb', '8a8b0ef43a4d7825a23ea817e7dd656dde7fe3fe58e87d20c7fdfd4fc72f373c'),
    ('libsdl2-dev_2.0.14+dfsg2-3+deb11u1_arm64.deb', '77c694ed46f03290c61349261b03bf3765ec626454fcde22185326e939344845'),
]
for name, digest in packages:
    data = urllib.request.urlopen('https://archive.debian.org/debian/pool/main/libs/libsdl2/' + name, timeout=60).read()
    if hashlib.sha256(data).hexdigest() != digest: raise ValueError('SDK checksum mismatch')
    position = 8
    while position < len(data):
        header = data[position:position+60]
        size = int(header[48:58])
        if header[:16].decode().strip().startswith('data.tar'):
            with tarfile.open(fileobj=io.BytesIO(data[position+60:position+60+size])) as archive:
                for member in archive:
                    if not member.isfile() or not ('libSDL2' in member.name or '/SDL2/' in member.name): continue
                    relative = Path(member.name)
                    if relative.is_absolute() or '..' in relative.parts: raise ValueError('Unsafe archive path')
                    dest = args.destination / relative
                    dest.parent.mkdir(parents=True, exist_ok=True)
                    dest.write_bytes(archive.extractfile(member).read())
        position += 60 + size + size % 2
print('SDL2 SDK extracted to', args.destination)

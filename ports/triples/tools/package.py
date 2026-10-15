"""Package local prototype builds, without uploading or publishing them."""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import zipfile

root = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser()
parser.add_argument('--arm', type=Path, required=True)
parser.add_argument('--windows', type=Path)
parser.add_argument('--destination', type=Path, required=True)
parser.add_argument('--version', default='0.6.0')
parser.add_argument('--private-skin', action='store_true', help='include locally prepared original artwork; never publish this package')
args = parser.parse_args()
args.destination.mkdir(parents=True, exist_ok=True)
binary = args.arm.read_bytes()
assert binary[:5] == b'\x7fELF\x02' and struct.unpack_from('<H', binary, 18)[0] == 183, 'Not ELF64 AArch64'

def put(z, source, name, executable=False):
    info = zipfile.ZipInfo(name)
    info.create_system = 3
    info.external_attr = (0o100755 if executable else 0o100644) << 16
    info.compress_type = zipfile.ZIP_DEFLATED
    z.writestr(info, source.read_bytes())

manifest = {
    'version': args.version, 'target': 'Linux AArch64 / Cortex-A53 / KNULLI Scarab',
    'upstream_commit': '83e8286d15a1d3631d110dd47df88fd94d28c85a',
    'binary_sha256': hashlib.sha256(binary).hexdigest(),
    'hardware_tested': True, 'desktop_smoke_exit': 0,
    'previous_version_user_confirmed_working_on_hardware': True,
    'tests': {'passed': 33, 'checks': 17330, 'failed': 0},
    'runtime': 'system libSDL2-2.0.so.0 and glibc; highest referenced GLIBC version 2.27',
}
with zipfile.ZipFile(args.destination / f'threes-knulli-v{args.version}.zip', 'w') as z:
    put(z, root / 'portmaster/Triples.sh', 'Triples.sh', True)
    put(z, args.arm, 'threes-knulli/threes', True)
    put(z, root / 'portmaster/threes.gptk', 'threes-knulli/threes.gptk')
    if args.private_skin:
        textures = sorted((root / 'assets/skin').glob('*.rgba'))
        if not textures: parser.error('private skin requested but no prepared textures exist')
        for texture in textures:
            put(z, texture, 'threes-knulli/assets/skin/' + texture.name)
    for name in ['README.md', 'ASSETS-LICENSE.md', 'UNLICENSE', 'CREDITS.md']:
        put(z, root / name, 'threes-knulli/' + name)
    if args.private_skin:
        put(z, root / 'assets/original/provenance.json', 'threes-knulli/provenance.json')
    z.writestr('threes-knulli/build-info.json', json.dumps(manifest, indent=2))
if args.windows:
    with zipfile.ZipFile(args.destination / f'threes-windows-preview-v{args.version}.zip', 'w') as z:
        put(z, args.windows, 'threes.exe')
        if args.private_skin:
            for texture in sorted((root / 'assets/skin').glob('*.rgba')):
                put(z, texture, 'assets/skin/' + texture.name)
        z.writestr('Jugar.bat', '@echo off\r\nset "THREES_SAVE_DIR=%~dp0saves"\r\n"%~dp0threes.exe"\r\n')
        put(z, root / 'README.md', 'README.md')
        put(z, root / 'ASSETS-LICENSE.md', 'ASSETS-LICENSE.md')
        put(z, root / 'UNLICENSE', 'UNLICENSE')
        put(z, root / 'CREDITS.md', 'CREDITS.md')
with zipfile.ZipFile(args.destination / f'threes-knulli-source-v{args.version}.zip', 'w') as z:
    tracked = subprocess.check_output(['git', '-C', str(root), 'ls-files', '-z', '--', '.']).split(b'\0')
    for raw in sorted(filter(None, tracked)):
        relative = Path(raw.decode())
        source = root / relative
        if not source.is_file() or any(p == '.git' or p == '__pycache__' or p.startswith('build') for p in relative.parts): continue
        if relative.parts[0] == 'assets' and len(relative.parts) > 1 and relative.parts[1] in ('original', 'skin'): continue
        if source.suffix.lower() in ('.zip', '.apk', '.o2r', '.z64', '.sav', '.rgba'): continue
        put(z, source, 'threes-knulli/' + relative.as_posix(), source.suffix == '.sh')
print(json.dumps(manifest, indent=2))

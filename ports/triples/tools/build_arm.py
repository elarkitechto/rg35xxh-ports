"""Cross-build with Zig 0.14.1 and an extracted Debian bullseye ARM64 SDL2 SDK.

The Debian library is a link reference only. The package uses KNULLI's SDL2.
Usage: python tools/build_arm.py --zig /path/to/zig --sdk /path/to/sdk --out build-arm
"""
import argparse
from pathlib import Path
import shutil
import subprocess

root = Path(__file__).resolve().parents[1]
parser = argparse.ArgumentParser()
parser.add_argument('--zig', required=True)
parser.add_argument('--sdk', type=Path, required=True)
parser.add_argument('--out', type=Path, required=True)
parser.add_argument('--private-skin', action='store_true', help='copy local original-art skin into this private build')
args = parser.parse_args()
out = args.out.resolve()
out.mkdir(parents=True, exist_ok=True)
embedded = out / 'embedded.cpp'
text = '#include "resources/embedded.hpp"\n'
for symbol, filename in [('font_inter_bold', 'Inter-Bold.ttf'), ('font_jua_regular', 'Jua-Regular.ttf')]:
    data = (root / 'assets/fonts' / filename).read_bytes()
    text += f'const unsigned char {symbol}_data[] = {{' + ','.join(map(str, data)) + '};\n'
    text += f'const unsigned long {symbol}_size = {len(data)}ul;\n'
embedded.write_text(text)
sources = []
for directory in ['game', 'render', 'input', 'audio']:
    sources += [p for p in (root / 'src' / directory).glob('*.cpp') if p.name != 'mixer.cpp']
sources += [root / 'src/main.cpp', root / 'src/app.cpp', root / 'src/platform/persistence_posix.cpp', embedded]
sdk = args.sdk.resolve()
command = [str(Path(args.zig).resolve()), 'c++', '-target', 'aarch64-linux-gnu.2.31', '-mcpu=cortex_a53',
           '-std=c++20', '-O2', '-DSDL_MAIN_HANDLED', '-I' + str(root / 'src'), '-I' + str(root / 'third_party'),
           '-I' + str(sdk / 'usr/include/SDL2'), '-I' + str(sdk / 'usr/include/aarch64-linux-gnu'),
           *map(str, sources), str(sdk / 'usr/lib/aarch64-linux-gnu/libSDL2-2.0.so.0.14.0'),
           '-Wl,--allow-shlib-undefined', '-o', str(out / 'threes')]
subprocess.run(command, check=True)
if args.private_skin:
    skin = root / 'assets/skin'
    if not skin.is_dir() or not list(skin.glob('*.rgba')):
        parser.error('private skin requested but no prepared textures exist')
    shutil.copytree(skin, out / 'assets/skin', dirs_exist_ok=True)
print('Built ARM64 binary:', out / 'threes')

#!/usr/bin/env python3
"""Package the source-built runtime; never include owner-supplied game files."""
import argparse
import hashlib
import json
from pathlib import Path
from zipfile import ZipFile, ZipInfo, ZIP_DEFLATED

p = argparse.ArgumentParser()
p.add_argument('--runtime', type=Path, required=True)
p.add_argument('--media', type=Path)
p.add_argument('--output', type=Path, required=True)
a = p.parse_args()
root = Path(__file__).resolve().parents[1]
files = {
    'Threes!.sh': root / 'portmaster/Threes!.sh',
    'threesandroid/install-data': root / 'portmaster/threesandroid/install-data',
    'threesandroid/runtime/threes-loader': a.runtime / 'threes-loader',
    'threesandroid/runtime/libOpenSLES.so': a.runtime / 'libOpenSLES.so',
    'threesandroid/README.md': root / 'README.md',
    'port.json': root / 'portmaster/port.json',
    'gameinfo.xml': root / 'portmaster/gameinfo.xml',
    'threesandroid/licenses/LICENSE': root / 'LICENSE',
    'threesandroid/licenses/NOTICE': root / 'NOTICE',
    'threesandroid/licenses/Khronos-permission-notice.h': root / 'source/runtime/include/KHR/khrplatform.h',
}
if a.media:
    files['threesandroid/cover.png'] = a.media / 'cover.png'
    files['cover.png'] = a.media / 'cover.png'
    files['screenshot.png'] = a.media / 'screenshot.png'
for source in files.values():
    if not source.is_file() or not source.stat().st_size:
        p.error(f'missing package input: {source}')
json.loads(files['port.json'].read_text())
a.output.parent.mkdir(parents=True, exist_ok=True)
with ZipFile(a.output, 'w', compression=ZIP_DEFLATED) as z:
    for name, source in files.items():
        info = ZipInfo(name)
        executable = name.endswith('.sh') or name.endswith('/threes-loader')
        info.external_attr = (0o100755 if executable else 0o100644) << 16
        info.compress_type = ZIP_DEFLATED
        z.writestr(info, source.read_bytes())
    z.writestr('threesandroid/version.txt', '0.1.0-rc1\n')
    checksums=''.join(hashlib.sha256((a.runtime / name).read_bytes()).hexdigest()+'  '+name+'\n' for name in ['threes-loader','libOpenSLES.so'])
    z.writestr('threesandroid/runtime/runtime.sha256',checksums)
print(a.output)

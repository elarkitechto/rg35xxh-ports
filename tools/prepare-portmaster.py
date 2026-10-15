#!/usr/bin/env python3
"""Validate and stage a built candidate in PortMaster's port/<name> layout."""
import argparse
import json
import sys
from pathlib import Path, PurePosixPath
from zipfile import ZipFile

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('port', choices=['altosadventure', 'crossyroad', 'threes'])
p.add_argument('--package', type=Path)
p.add_argument('--check-only', action='store_true', help='validate without extracting')
p.add_argument('--output', type=Path, default=Path('.local/portmaster/port'))
a = p.parse_args()
archive = a.package or Path('.local/releases') / (a.port + '.zip')
with ZipFile(archive) as z:
    names = set(z.namelist())
    for name in names:
        path = PurePosixPath(name)
        if path.is_absolute() or '..' in path.parts:
            p.error('unsafe archive path: ' + name)
        if (path.suffix.lower() in ['.apk', '.xapk', '.sav', '.log', '.ppm']
                or path.name in ['libunity.so', 'libil2cpp.so', 'libMali.so', '.DS_Store', 'log.txt']
                or path.name.startswith('._') or '.before-' in path.name
                or any(part in ['gamedata', 'prefs', 'saves'] for part in path.parts)):
            p.error('owner data must not be distributed: ' + name)
    for name in ['port.json', 'README.md', 'gameinfo.xml', 'screenshot.png']:
        if name not in names:
            p.error('missing required package file: ' + name)
    info = json.loads(z.read('port.json'))
    if info['name'] != a.port + '.zip':
        p.error('port name and archive name differ')
    items = info['items']
    if a.port not in [i.rstrip('/') for i in items]:
        p.error('missing matching data directory')
    for item in items:
        if item.endswith('/') or item == a.port:
            if not any(n.startswith(item.rstrip('/') + '/') for n in names):
                p.error('missing data directory: ' + item)
        elif item not in names:
            p.error('missing launcher: ' + item)
    source = Path(__file__).resolve().parents[1] / 'ports' / a.port / 'portmaster'
    for name in [i for i in items if i.endswith('.sh')] + [a.port + '/install-data']:
        if name not in names or z.read(name) != (source / name).read_bytes():
            p.error('stale or missing launcher/preparer; rebuild the complete package: ' + name)
    if a.check_only:
        print('Package verified: ' + str(archive))
        sys.exit(0)
    target = a.output / a.port
    if target.exists():
        p.error('output already exists; choose a fresh output directory')
    target.mkdir(parents=True)
    z.extractall(target)
    for name in names:
        if name.endswith('.sh'):
            (target / name).chmod(0o755)
print(target)

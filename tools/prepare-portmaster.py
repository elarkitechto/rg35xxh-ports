#!/usr/bin/env python3
"""Validate and stage a built candidate in PortMaster's port/<name> layout."""
import argparse
import json
from pathlib import Path, PurePosixPath
from zipfile import ZipFile

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('port', choices=['altosadventure', 'crossyroad', 'threes'])
p.add_argument('--package', type=Path)
p.add_argument('--output', type=Path, default=Path('.local/portmaster/port'))
a = p.parse_args()
archive = a.package or Path('.local/releases') / (a.port + '.zip')
with ZipFile(archive) as z:
    names = set(z.namelist())
    for name in names:
        path = PurePosixPath(name)
        if path.is_absolute() or '..' in path.parts:
            p.error('unsafe archive path: ' + name)
        if path.suffix.lower() in ['.apk', '.xapk', '.sav']:
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
    target = a.output / a.port
    if target.exists():
        p.error('output already exists; choose a fresh output directory')
    target.mkdir(parents=True)
    z.extractall(target)
    for name in names:
        if name.endswith('.sh'):
            (target / name).chmod(0o755)
print(target)

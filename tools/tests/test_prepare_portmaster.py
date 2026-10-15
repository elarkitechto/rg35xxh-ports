"""Reject stale/mixed packages and accidental owner data before staging."""
from pathlib import Path
import json
import subprocess
import tempfile
import unittest
from zipfile import ZipFile

ROOT = Path(__file__).resolve().parents[2]

class PackageTests(unittest.TestCase):
    def check(self, replacement=None):
        source = ROOT / 'ports/threes/portmaster'
        with tempfile.TemporaryDirectory() as temp:
            package = Path(temp) / 'threes.zip'
            files = {name: (source / name).read_bytes() for name in
                     ['port.json', 'gameinfo.xml', 'Threes!.sh', 'threes/install-data']}
            files.update({'README.md': b'test readme', 'screenshot.png': b'test media',
                          'threes/runtime/threes-loader': b'test runtime'})
            if replacement:
                files.update(replacement)
            with ZipFile(package, 'w') as z:
                for name, data in files.items():
                    z.writestr(name, data)
            return subprocess.run(['python3', str(ROOT / 'tools/prepare-portmaster.py'),
                                   'threes', '--package', str(package), '--check-only'],
                                  capture_output=True, text=True)

    def test_current_launcher_and_preparer_are_accepted(self):
        self.assertEqual(self.check().returncode, 0)

    def test_stale_preparer_is_rejected(self):
        result = self.check({'threes/install-data': b'#!/bin/sh\nSYSTEM_ROOT=$2\n'})
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('stale or missing launcher/preparer', result.stderr)

    def test_owner_files_and_development_debris_are_rejected(self):
        for name in ['threes/game.apk', 'threes/runtime/libunity.so',
                     'threes/log.txt', 'threes/runtime/frame-420.ppm', 'threes/.DS_Store', 'threes/saves/progress']:
            with self.subTest(name=name):
                self.assertNotEqual(self.check({name: b'not distributable'}).returncode, 0)

if __name__ == '__main__':
    unittest.main()

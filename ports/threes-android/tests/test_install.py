"""Setup failure and repeated-launch contracts, without proprietary fixtures."""
from pathlib import Path
import subprocess
import tempfile
import unittest

INSTALL = Path(__file__).resolve().parents[1] / 'portmaster/threesandroid/install-data'
DIGEST = '99dcba5f8dbcbc4294507c6b30d9f0719ae4127228186e9a471ecdb45a77c880'

class SetupTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.save = self.root / 'saves/progress'
        self.save.parent.mkdir()
        self.save.write_bytes(b'keep this progress')

    def tearDown(self):
        self.assertEqual(self.save.read_bytes(), b'keep this progress')
        self.temp.cleanup()

    def run_setup(self):
        return subprocess.run(['sh', str(INSTALL), str(self.root)], capture_output=True, text=True)

    def test_missing_archive_changes_no_game_data(self):
        result = self.run_setup()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('Missing', result.stderr)
        self.assertFalse((self.root / 'gamedata').exists())

    def test_wrong_archive_changes_no_game_data(self):
        (self.root / 'threes-free-3-1-24576.xapk').write_bytes(b'wrong archive')
        result = self.run_setup()
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('Wrong XAPK', result.stderr)
        self.assertFalse((self.root / 'gamedata').exists())

    def test_installed_game_starts_without_reextracting_archive(self):
        for relative in ['gamedata/assets/bin/Data/globalgamemanagers',
                         'gamedata/assets/bin/Data/Managed/Metadata/global-metadata.dat',
                         'runtime/libil2cpp.so', 'runtime/libunity.so',
                         'runtime/threes-loader', 'runtime/libOpenSLES.so']:
            file = self.root / relative
            file.parent.mkdir(parents=True, exist_ok=True)
            file.write_bytes(b'installed fixture')
        (self.root / 'runtime/threes-loader').chmod(0o755)
        marker = self.root / 'gamedata/.xapk.sha256'
        marker.write_text(DIGEST + '\n')
        before = marker.stat().st_mtime_ns
        result = self.run_setup()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('already installed', result.stdout)
        self.assertEqual(marker.stat().st_mtime_ns, before)

if __name__ == '__main__':
    unittest.main()

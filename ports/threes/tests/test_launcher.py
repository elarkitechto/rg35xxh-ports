from pathlib import Path
import os
import shutil
import subprocess
import tempfile
import unittest

SOURCE = Path(__file__).resolve().parents[1] / 'portmaster/Threes!.sh'

class LauncherTests(unittest.TestCase):
    def test_first_launch_prepares_runtime_and_removes_test_inputs(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            launcher = root / SOURCE.name
            shutil.copy2(SOURCE, launcher)
            port = root / 'threes'
            port.mkdir()
            control = root / 'PortMaster'
            control.mkdir()
            (control / 'control.txt').write_text('''
CFW_NAME=TEST
DEVICE_ARCH=aarch64
get_controls() { sdl_controllerconfig='verified-mapping'; }
pm_finish() { echo finished >> "$RESULT"; }
''')
            (port / 'install-data').write_text('''#!/bin/sh
mkdir -p "$1/runtime"
cat > "$1/runtime/threes-loader" <<'LOADER'
#!/bin/sh
[ -z "${THREES_TEST_INPUT:-}" ] || exit 90
[ -z "${THREES_TEST_CONTROLS:-}" ] || exit 91
[ -z "${THREES_TEST_ROTATE:-}" ] || exit 92
[ -z "${THREES_FRAMES:-}" ] || exit 93
printf '%s\\n' "$SDL_GAMECONTROLLERCONFIG" "$THREES_DATA_DIR" "$THREES_SAVE_DIR" > "$RESULT"
LOADER
chmod +x "$1/runtime/threes-loader"
''')
            result_file = root / 'result'
            save = root / 'existing-save'
            save.mkdir()
            (save / 'progress').write_text('preserved')
            env = dict(os.environ, THREES_PORTMASTER_DIR=str(control),
                       THREES_SAVE_DIR=str(save), RESULT=str(result_file),
                       THREES_TEST_INPUT='1', THREES_TEST_CONTROLS='1',
                       THREES_TEST_ROTATE='1', THREES_FRAMES='1', THREES_ERROR_DELAY='0')
            result = subprocess.run(['bash',str(launcher)],env=env,capture_output=True,text=True,timeout=10)
            self.assertEqual(result.returncode,0,result.stderr)
            self.assertEqual(result_file.read_text().splitlines(),
                             ['verified-mapping',str(port / 'gamedata'),str(save),'finished'])
            self.assertEqual((save / 'progress').read_text(),'preserved')

if __name__ == '__main__':
    unittest.main()

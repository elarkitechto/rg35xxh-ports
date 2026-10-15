#!/usr/bin/env python3
"""Exercise the real launcher against a small PortMaster/loader fixture."""
import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import time
import signal
import unittest

SOURCE = Path(__file__).resolve().parents[1] / 'portmaster' / "Alto's Adventure.sh"


class LauncherTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='alto launcher ')
        self.root = Path(self.temp.name)
        self.port = self.root / 'altosadventure'
        self.port.mkdir()
        self.control = self.root / 'PortMaster'
        self.control.mkdir()
        self.trace = self.root / 'trace'
        self.launcher = self.root / SOURCE.name
        shutil.copyfile(SOURCE, self.launcher)
        (self.control / 'control.txt').write_text('''
CFW_NAME=TEST
DEVICE_ARCH=aarch64
get_controls() { sdl_controllerconfig='test-controller-mapping'; echo controls >> "$TRACE"; }
pm_finish() { echo cleanup >> "$TRACE"; }
pm_platform_helper() { echo helper >> "$TRACE"; }
pm_message() { echo "message:$*" >> "$TRACE"; }
''')
        (self.port / 'install-data').write_text('''#!/bin/sh
mkdir -p "$ALTO_RUNTIME_DIR"
cat > "$ALTO_RUNTIME_DIR/alto-loader" <<'LOADER'
#!/bin/sh
printf '%s\\n' "$ALTO_DATA_DIR" "$SDL_GAMECONTROLLERCONFIG" "$ALTO_RUNTIME_DIR" > "$RESULT"
[ -z "${ALTO_DIALOG_SMOKE:-}" ] || exit 91
[ -z "${ALTO_AUTOPRESS:-}" ] || exit 92
exit "${TEST_EXIT:-0}"
LOADER
chmod +x "$ALTO_RUNTIME_DIR/alto-loader"
''')
        self.env = dict(os.environ, ALTO_PORTMASTER_DIR=str(self.control),
                        TRACE=str(self.trace), RESULT=str(self.root / 'result'),
                        ALTO_DATA_DIR=str(self.root / 'portable data'),
                        ALTO_DIALOG_SMOKE='1', ALTO_AUTOPRESS='1', ALTO_ERROR_DELAY='0')

    def tearDown(self):
        self.temp.cleanup()

    def run_launcher(self):
        return subprocess.run(['bash', str(self.launcher)], env=self.env,
                              capture_output=True, text=True, timeout=15)

    def test_controls_paths_helpers_and_diagnostics(self):
        result = self.run_launcher()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertNotIn('bad substitution', result.stderr)
        self.assertEqual((self.root / 'result').read_text().splitlines(), [
            self.env['ALTO_DATA_DIR'], 'test-controller-mapping', str(self.port / 'runtime')])
        self.assertEqual(self.trace.read_text().splitlines(), ['controls', 'helper', 'cleanup'])

    def test_fresh_install_defaults_to_portable_data(self):
        self.env.pop('ALTO_DATA_DIR')
        result = self.run_launcher()
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertNotIn('bad substitution', result.stderr)
        self.assertEqual((self.root / 'result').read_text().splitlines()[0],
                         str(self.port / 'gamedata'))

    def test_cfw_mod_is_loaded(self):
        (self.control / 'mod_TEST.txt').write_text('echo cfw-mod >> "$TRACE"\n')
        self.assertEqual(self.run_launcher().returncode, 0)
        self.assertEqual(self.trace.read_text().splitlines()[0], 'cfw-mod')

    def test_loader_failure_is_preserved_and_cleaned(self):
        self.env['TEST_EXIT'] = '7'
        self.assertEqual(self.run_launcher().returncode, 7)
        self.assertEqual(self.trace.read_text().splitlines()[-1], 'cleanup')

    def test_missing_apk_setup_failure_reports_and_cleans(self):
        (self.port / 'install-data').write_text('#!/bin/sh\necho missing-apk >&2\nexit 1\n')
        self.assertEqual(self.run_launcher().returncode, 1)
        self.assertIn('missing-apk', (self.port / 'install.log').read_text())
        self.assertEqual(self.trace.read_text().splitlines()[-1], 'cleanup')
        self.assertFalse((self.root / 'result').exists())

    def test_wrong_architecture_does_not_install(self):
        with (self.control / 'control.txt').open('a') as stream:
            stream.write('\nDEVICE_ARCH=armhf\n')
        self.assertEqual(self.run_launcher().returncode, 1)
        self.assertFalse((self.port / 'runtime').exists())
        self.assertEqual(self.trace.read_text().splitlines()[-1], 'cleanup')

    def test_signal_cleans_up_running_loader(self):
        (self.port / 'install-data').write_text("""#!/bin/sh
mkdir -p "$ALTO_RUNTIME_DIR"
cat > "$ALTO_RUNTIME_DIR/alto-loader" <<'LOADER'
#!/bin/sh
echo $$ > "$RESULT"
exec sleep 20
LOADER
chmod +x "$ALTO_RUNTIME_DIR/alto-loader"
""")
        process = subprocess.Popen(['bash', str(self.launcher)], env=self.env,
                                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        try:
            for _ in range(100):
                if (self.root / 'result').exists():
                    break
                time.sleep(0.02)
            child = int((self.root / 'result').read_text())
            process.send_signal(signal.SIGTERM)
            self.assertEqual(process.wait(timeout=5), 143)
            with self.assertRaises(ProcessLookupError):
                os.kill(child, 0)
            self.assertEqual(self.trace.read_text().splitlines()[-1], 'cleanup')
        finally:
            if process.poll() is None:
                process.kill()
                process.wait()

    def test_no_portmaster_fails_without_running_installer(self):
        (self.control / 'control.txt').unlink()
        result = self.run_launcher()
        self.assertEqual(result.returncode, 1)
        self.assertIn('requires PortMaster', result.stderr)
        self.assertFalse((self.port / 'runtime').exists())


if __name__ == '__main__':
    unittest.main()

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

SOURCE = Path(__file__).resolve().parents[1] / 'portmaster' / "Crossy Road.sh"


class LauncherTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='crossy launcher ')
        self.root = Path(self.temp.name)
        self.port = self.root / 'crossyroad'
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
pm_finish() { [ -z "${LD_PRELOAD:-}" ] || echo preload-leaked >> "$TRACE"; echo cleanup >> "$TRACE"; }
pm_platform_helper() { [ -z "${LD_PRELOAD:-}" ] || echo preload-leaked >> "$TRACE"; echo helper >> "$TRACE"; }
pm_message() { echo "message:$*" >> "$TRACE"; }
''')
        (self.port / 'install-data').write_text('''#!/bin/sh
mkdir -p "$CROSSY_RUNTIME_DIR"
cat > "$CROSSY_RUNTIME_DIR/crossy-loader" <<'LOADER'
#!/bin/sh
printf '%s\\n' "$CROSSY_DATA_DIR" "$SDL_GAMECONTROLLERCONFIG" "$CROSSY_RUNTIME_DIR" > "$RESULT"
[ "$HK_PTHREAD_SHIM:$HK_SDLPAD:$CROSSY_NATIVE_INPUT" = "1:1:1" ] || exit 94
[ -z "${CROSSY_TEST_PAUSE_HUD:-}" ] || exit 91
[ -z "${CROSSY_AUTOPRESS_UP:-}" ] || exit 92
exit "${TEST_EXIT:-0}"
LOADER
chmod +x "$CROSSY_RUNTIME_DIR/crossy-loader"
''')
        self.env = dict(os.environ, CROSSY_PORTMASTER_DIR=str(self.control),
                        TRACE=str(self.trace), RESULT=str(self.root / 'result'),
                        CROSSY_DATA_DIR=str(self.root / 'portable data'),
                        CROSSY_TEST_PAUSE_HUD='1', CROSSY_AUTOPRESS_UP='1', CROSSY_ERROR_DELAY='0')

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
            self.env['CROSSY_DATA_DIR'], 'test-controller-mapping', str(self.port / 'runtime')])
        self.assertEqual(self.trace.read_text().splitlines(), ['controls', 'helper', 'cleanup'])

    def test_fresh_install_defaults_to_portable_data(self):
        self.env.pop('CROSSY_DATA_DIR')
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
mkdir -p "$CROSSY_RUNTIME_DIR"
cat > "$CROSSY_RUNTIME_DIR/crossy-loader" <<'LOADER'
#!/bin/sh
echo $$ > "$RESULT"
exec sleep 20
LOADER
chmod +x "$CROSSY_RUNTIME_DIR/crossy-loader"
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



    def prepare_real_installer(self):
        """Exercise the production launcher/preparer contract, including runtime placement."""
        import hashlib
        shutil.copyfile(SOURCE.parent / 'crossyroad/install-data', self.port / 'install-data')
        data = Path(self.env['CROSSY_DATA_DIR'])
        for relative in ['assets/bin/Data/data.unity3d',
                         'assets/bin/Data/Managed/Metadata/global-metadata.dat', 'prefs/progress']:
            path = data / relative
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_bytes(b'preserved game data')
        (data / '.crossy-apk.sha256').write_text('1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb\n')
        runtime = self.port / 'runtime'
        runtime.mkdir()
        library_checksums = []
        for name in ['libunity.so', 'libil2cpp.so']:
            payload = b'installed library fixture'
            (runtime / name).write_bytes(payload)
            library_checksums.append(hashlib.sha256(payload).hexdigest() + '  ' + name + '\n')
        (self.port / 'crossy-libraries.sha256').write_text(''.join(library_checksums))
        (runtime / 'crossy-loader').write_text('#!/bin/sh\necho real-installer-started > "$RESULT"\n')
        (runtime / 'crossy-loader').chmod(0o755)
        names = ['crossy-loader', 'libOpenSLES.so', 'libcrossy-jni-native.so']
        for name in names[1:]:
            (runtime / name).write_bytes(b'loader dependency fixture')
        (runtime / 'runtime.sha256').write_text(''.join(
            hashlib.sha256((runtime / name).read_bytes()).hexdigest() + '  ' + name + '\n'
            for name in names))
        return data, runtime

    def test_real_preparer_places_runtime_where_launcher_executes(self):
        data, runtime = self.prepare_real_installer()
        result = self.run_launcher()
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertEqual((self.root / 'result').read_text().strip(), 'real-installer-started')
        self.assertEqual((runtime / 'crossy-loader').read_text(), '#!/bin/sh\necho real-installer-started > "$RESULT"\n')
        self.assertEqual((data / 'prefs/progress').read_bytes(), b'preserved game data')

    def test_real_preparer_rejects_incomplete_bundle_before_launch(self):
        data, runtime = self.prepare_real_installer()
        (runtime / 'crossy-loader').unlink()
        result = self.run_launcher()
        self.assertNotEqual(result.returncode, 0)
        self.assertFalse((self.root / 'result').exists())
        self.assertEqual((data / 'prefs/progress').read_bytes(), b'preserved game data')

if __name__ == '__main__':
    unittest.main()

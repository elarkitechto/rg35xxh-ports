# Development and releases

Keep each port in its own directory and use a focused branch for each change.
Follow the commit conventions in [CONTRIBUTING.md](../CONTRIBUTING.md).

Enable local checks with `git config core.hooksPath .githooks`. The hook checks
tracked files, script syntax, metadata and launcher tests.

Before preparing a release:

1. Confirm the provenance and license of each distributed component.
2. Test fresh installation, updates that preserve saves, controls, audio, saving and exit.
3. Record the device, firmware, resolution, game version and game-file hash.
4. Run local checks and review the staged changes.
5. Tag each port separately once its package and compatibility limits are verified.

Build instructions are in [BUILD.md](BUILD.md). PortMaster submission preparation
is in [PORTMASTER.md](PORTMASTER.md).

When updating a handheld, deploy the launcher, automatic data preparer, runtime
binaries and checksum manifests from the same package. Updating a launcher or
renaming an older installer alone can leave incompatible paths or arguments.
After deployment, run the installed launcher with isolated test data and verify
its startup log before replacing a known working installation.

After graphical diagnostics, clear game-specific environment variables before
restarting EmulationStation. Otherwise the menu can pass temporary data/save
paths into a later normal launch. Verify its process environment after restart.

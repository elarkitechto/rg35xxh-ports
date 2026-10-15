# Contributing

1. Use a focused branch for each change, such as `fix/crossyroad-input` or `docs/installation`.
2. Write descriptive commits: `feat(crossyroad): ...`, `fix(threes): ...` or `docs: ...`.
3. Run `sh .githooks/pre-commit` before submitting changes. Enable automatic checks with `git config core.hooksPath .githooks`.
4. Record the device, firmware, exact game version and tests performed. Verify installation, updates, controls, audio, saves and exit behavior as applicable.
5. Preserve license notices and credit upstream code. Keep public documentation in English.

Do not commit APKs, extracted game data, saves, firmware drivers, compiled
binaries, user logs or `.local/` files. See [attribution](ATTRIBUTION.md) for
component ownership and licenses.

See [PortMaster packaging](docs/PORTMASTER.md) for candidate preparation and
submission checks.

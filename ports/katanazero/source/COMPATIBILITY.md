# Compatibility and validation

## Saves

Progress and options remain under `katanazero/save/`, with local slot storage in
`save/local-slots/`. Keep that directory when updating. No initial save is shipped.
The bridge uses a local profile. Netflix login, cloud synchronization, leaderboards
and other online services are not implemented. The local bridge is adapted from
behavior observed in a modified Android distribution; it does not execute that
APK's Java implementation or perform genuine Netflix authentication.

## Compatibility and troubleshooting

User-confirmed: native controls and two levels. Initial data write/read and
checksums were verified. Audio has unresolved OGG/OpenAL warnings; clean shutdown
and full progression persistence still require validation. A SIGSEGV was observed
after saving options on exit. Earlier termination was not proven to be an OOM.

The official 1.1.10 XAPK has byte-identical native engine and game data to the
runtime used during development. Fresh on-device setup from the official XAPK was verified through the title
and opening scene using the packaged launcher, without a seed save. Gameplay,
audible output and F+Start still need acceptance on this clean installation.
See `install.log` for setup errors and `log.txt` for runtime errors. Unsupported
versions are rejected before replacing game files. Modified APK files are not a
supported installation input.

## Build

See [source and packaging](source/README.md). This repository contains source,
patches and package metadata; it does not contain game files, saves or runtime
binaries. No release download is advertised until the package passes acceptance.


## Controller presentation

Confirmed on the handheld: maintaining global gamepadMode as a scalar boolean
eliminates the touch overlay on initial load. The adapter accepts real and boolean
RValue kinds. Duplicate keyboard/mouse gameplay mappings are disabled.
The display remains widescreen without cropping.

The official XAPK clean-install path reached the title and opening scene.
The player reports gameplay with Bloom and shaders enabled; Bloom is not a
mandatory disable setting. Full-game stability and the exit fault remain open.

## Release status

The player package is distributed in GitHub Releases with the corresponding
GMLoader source in a separate archive. Players install only katanazero.zip and
supply their own official 1.1.10 ARM64 XAPK. Controller-mode presentation was
confirmed on the handheld after the boolean-type fix. The package remains
experimental because audio warnings and an exit fault are unresolved.

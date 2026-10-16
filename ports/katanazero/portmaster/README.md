# Katana ZERO

Community Android compatibility port for **RG35XX-H / KNULLI**, using
[GMLoader-next](https://github.com/JohnnyonFlame/gmloader-next). Experimental;
not yet accepted into the PortMaster catalog.

## Install

1. Extract the built `katanazero.zip` into your **Ports** folder.
2. Supply your **Katana ZERO Netflix 1.1.10 ARM64 XAPK**, named
   `katana-zero-1.1.10.xapk`, in `ports/katanazero/`.
3. Launch **Katana ZERO**. The launcher validates and prepares the game files
   automatically. No additional setup script or handheld compiler is needed.

The XAPK must contain the base APK, `config.arm64_v8a.apk` and
`DynamicAssetDeliveryFiles.apk`. Setup verifies the native engine, game data and
options by SHA-256. Game files are not included or downloaded by this project.
A valid copy and applicable game/subscription rights remain the player's responsibility.

## Controls and display

Native controller input is enabled. Follow the game's button prompts.
**F + Start** uses the standard PortMaster exit combination. Keep the original
widescreen framing: filling 4:3 by cropping hides level geometry and HUD elements.
The loader's placeholder pattern is replaced by black until the game logo.
Disable **Bloom** in the game's graphics options; two complete levels were
reported playable with it disabled. This does not establish full-game stability.

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

See [source and packaging](https://github.com/elarkitechto/rg35xxh-ports/tree/main/ports/katanazero/source). This repository contains source,
patches and package metadata; it does not contain game files, saves or runtime
binaries. No release download is advertised until the package passes acceptance.

## Credits and license

Katana ZERO is by Askiisoft, published by Devolver Digital. Android distribution
and SDK are associated with Netflix. GMLoader-next is by JohnnyonFlame and its
contributors, GPLv2. Loader patches, integration and packaging: **elarkitechto**.
This independent community project is not endorsed by those parties or PortMaster.
The loader patch is GPLv2; see [LICENSE](katanazero/LICENSE). Game assets and branding are
excluded from that license.

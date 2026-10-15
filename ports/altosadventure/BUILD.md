# Alto's Adventure for RG35XX-H / KNULLI

This port runs the Android 1.8.8 release using an ARM64 compatibility layer. Supply your own APK; game assets are not included. See the [installation notes and controls](portmaster/README.md).

## Build

The build requires Zig 0.13.0, Python 3, SDL2 headers, `pkg-config`, `zip`, and link libraries from the target KNULLI firmware (`libSDL2.so`, `libmali.so`, `libz.so`). Point `ALTO_LINK_LIBS` to the directory holding those libraries. Set `ZIG_BIN` and `SDL_INCLUDE_DIR` if they are outside the normal search path.

Supply `ALTO_MEDIA_DIR` with a distinct 4:3 `screenshot.png` (at least 640×480) and vertical box-art `cover.png`. See [cover guidelines](../../docs/COVERS.md). Then run from the repository root:

```sh
ALTO_LINK_LIBS=/path/to/knulli-libraries ALTO_MEDIA_DIR=/path/to/media sh tools/build-alto-local.sh
```

The output is `.local/releases/altosadventure.zip`. Extract it into the handheld's Ports directory, add the APK as described above, and launch the game.

## Source and status

`source/` contains the loader, input integration and audio bridge. `policy-patches.json` records the guarded changes for the pinned APK. [Validation](VALIDATION.md) records the tested device and remaining limits; [offline patches](OFFLINE-PATCHES.md) records disabled online services.

This build has been tested on RG35XX-H / KNULLI for gameplay, menu navigation, audio output and local save/reload. Extended play and physical vibration need further testing. The original game and its assets belong to Snowman. The framework's upstream copyright and license notices are in `NOTICE` and `LICENSE`; port adaptation by [elarkitechto](https://github.com/elarkitechto).

Controller routing and supported modal types are documented in [INPUT.md](INPUT.md), including the single-button Thanks dialog regression test.

## PortMaster testing candidate

[0.2.0-rc2 testing guide](TESTING.md) records the compatibility matrix and one-launch setup. [Discord draft](DISCORD-TESTING-DRAFT.md) is prepared but not posted. PortMaster integration is enabled; proprietary Mali backend dependencies and other CFWs remain to be validated.

SDL headers must declare `SDL_GameControllerHasRumble`; older SDK headers cannot build the haptic support. `SDL_INCLUDE_DIR` points to the directory containing `SDL2/`.

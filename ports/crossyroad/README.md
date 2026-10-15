# Crossy Road — PortMaster candidate 0.2.0-rc1

Android4.5.0 ARM64 adaptation, tested on RG35XX-H/KNULLI at640×480. Thanks to Hipster Whale for creating the game. The generic runtime retains Apache-2.0/upstream notices; adaptation by elarkitechto. No APK, extracted game assets or system GPU driver is in this repository.

## Build and install

Install Zig0.13.0, Python3, SDL2 development headers, pkg-config and zip. Provide target ARM64 link inputs libSDL2.so/libmali.so/libz.so in an external directory and private media cover.png (vertical 2D box art) plus screenshot.png (4:3 gameplay, at least640×480). Set SDL_INCLUDE_DIR/ZIG_BIN if necessary.

```sh
CROSSY_LINK_LIBS=/path/to/target-libraries CROSSY_MEDIA_DIR=/path/to/private-media sh tools/build-crossyroad-local.sh
```

This single command builds the loader, JNI/controller preload and OpenSL library from source, validates media, stages licenses/metadata and creates `.local/releases/crossyroad-0.2.0-rc1.zip` plus its checksum. The historical pinned loader/components are no longer needed. Firmware link inputs are not bundled.

On the handheld, extract into Ports, supply crossy-road-4-5-0.apk inside crossyroad/ and launch **Crossy Road** once. Setup is automatic; no manual subfolder script or handheld compilation. Required APK SHA-256: `1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb`.

[TESTING.md](TESTING.md) contains compatibility and controls. [VALIDATION.md](VALIDATION.md) separates stable history from the reconstructed candidate's tests. [Discord draft](DISCORD-TESTING-DRAFT.md) is prepared but not posted. Only RG35XX-H/KNULLI is currently verified; other CFWs and GPU backends need work/testing. This is not an official catalogue release.

Fresh data/runtime are port-local. Existing KNULLI saves under /userdata/system/cr are reused in place. The corner touch pause icon is hidden; Start retains pause/resume. The native SDL movement, menu, save and haptic bridge is preserved. Historical binary patch_loader_sdl.py and build_native.sh remain for research/legacy compatibility, not the candidate build chain.

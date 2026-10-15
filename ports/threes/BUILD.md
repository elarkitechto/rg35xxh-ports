# Build Threes!

Run from the repository root with Zig 0.14.1, Python 3 and a matching ARM64
sysroot. Set THREES_SDK_ROOT to its `usr/` directory (SDL2 and GLES headers).
THREES_LINK_LIBS must contain target `libSDL2.so`, `libmali.so` and `libz.so`.
Firmware libraries are link inputs only; they are not bundled.

Provide private media as vertical `cover.png` and a separate 4:3 gameplay
`screenshot.png` of at least 640×480. See [media rules](../../docs/COVERS.md).

```sh
THREES_SDK_ROOT=/path/to/sysroot/usr \
THREES_LINK_LIBS=/path/to/target-libraries \
THREES_MEDIA_DIR=/path/to/media \
ZIG_BIN=/path/to/zig sh tools/build-threes-local.sh
```

This builds the loader and OpenSL bridge and creates
`.local/releases/threes-0.2.0-rc2.zip`, its checksum and `threes.zip`.
The package includes the main launcher, automatic data preparer, compiled
runtime, metadata, media and notices. It does not include the APK, extracted
game data, Unity/IL2CPP libraries or firmware libraries.

The supported full 3.1.6144 APK includes ARM64 Unity and IL2CPP libraries.
Name it `threes-3.1.6144.apk` and copy it into the package's `threes/` directory.
The installer verifies the entire APK and its native libraries. The launcher
runs data preparation automatically before starting the game.

The package builder selects the paid profile. `THREES_EDITION=free` remains
available only for rebuilding the historical Free runtime directly through
`source/build.sh`; it is not compatible with the current package installer.

[Device validation and implementation notes](STATUS.md) · [User guide](README.md)

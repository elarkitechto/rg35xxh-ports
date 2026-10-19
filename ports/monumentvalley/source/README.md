# Build Monument Valley

Requires Zig 0.13, a target-compatible SDK with SDL2/GLES headers and the device's
link libraries. Do not redistribute firmware libraries or proprietary game data.

```sh
export ZIG_BIN=zig
export MONUMENT_SDK_ROOT=/path/to/sysroot/usr
export MONUMENT_LINK_LIBS=/path/to/target/link-libraries
export MONUMENT_MEDIA_DIR=/path/to/media
sh tools/build-monumentvalley-local.sh
```

The media directory contains a vertical `cover.png` and a 640×480 gameplay
`screenshot.png`. The build creates the final controller runtime, automatic
installer and sanitized package in `.local/releases/`. The player supplies the
APK documented in the install guide; conversion runs on the handheld.

Only the final gamepad adapter is included. Original Unity InputSystem actions
and gameplay remain in the supplied game. Runtime profiles check pinned ARM64
opcodes; the installer verifies the APK and game-library hashes.

Source and framework notices are supplied under Apache-2.0; see LICENSE/NOTICE.

## Media

The release cover uses ustwo games' [Monument Valley Steam library art](https://cdn.akamai.steamstatic.com/steam/apps/1927720/library_600x900_2x.jpg),
resized proportionally to 600×900 without embedded author/device metadata.
The screenshot is 640×480 gameplay from this Android port. Artwork and game
content retain their respective owners' rights.

Haptic hooks use one-instruction ARM64 detours with pinned prologue guards.
Only the first non-PC-relative instruction is copied to each trampoline; the
remaining original function executes unchanged. Scene sequence feedback matches
three explicit authored explosion/collapse names, not arbitrary substrings.
Effects expire in hardware; sequence cancellation and a safety timeout prevent
a stalled coroutine from leaving rumble active.

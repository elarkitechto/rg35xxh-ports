# Alto's Adventure

Native ARM64 adaptation of Android Alto 1.8.8 (Unity 2019.4.35f1), tested on RG35XX-H / KNULLI. Uses the existing Android Unity ELF/JNI/OpenSL/SDL framework, with game-specific offline patches and NGUI controller navigation. No dependency on Crossy Road at runtime.

## Build and install

On the development computer: install Zig 0.13.0, SDL2 development headers, Python3, zip and pkg-config. Supply the KNULLI ARM64 link libraries `libSDL2.so`, `libmali.so`, `libz.so` in an external directory. They are only link inputs, not redistributed in the package. Set `ZIG_BIN` if Zig is not in PATH and `SDL_INCLUDE_DIR` to the parent of SDL2 headers if needed.

Run **one command** from the repository root:

```sh
ALTO_LINK_LIBS=/path/to/knulli-libraries ALTO_MEDIA_DIR=/path/to/private-media sh tools/build-alto-local.sh
```

`ALTO_MEDIA_DIR` must contain a real 640×480 gameplay capture named `screenshot.png` and `cover.png` (the gameplay capture may serve as the cover). This single command compiles the loader and OpenSL bridge, verifies metadata, stages licenses and runtime hashes, and produces `.local/releases/altosadventure.zip`. No precompiled loader or manual subfolder build is required.

Extract that ZIP into the handheld's Ports directory, copy your APK into `altosadventure/`, and launch **Alto's Adventure**. All handheld preparation is automatic; [user instructions](portmaster/README.md).

## Implementation and validation

`source/src/main_alto.c` loads the original IL2CPP/Unity libraries and hooks input/haptics; `alto_ui.c` navigates the original NGUI and modal controls; `alto_policy.c` checks original instructions before applying the 45 offline patches. `policy-patches.json` records original opcodes. Generic runtime remains under Apache-2.0 with upstream notices. Game assets, original IL2CPP metadata dumps, APK, proprietary libraries, captures and saves are excluded from Git.

Gameplay, local save/reload, audio PCM, menu navigation, sliders, Settings tabs and Google Play suppression were tested. Frame pacing targets a total frame budget of 16.667ms instead of adding that delay after render. Short separate-profile samples measured 28.36/28.98 FPS with the old pacing and 47.15/50.03 FPS with the fix; scenes differ, so these are indicative rather than minimum FPS promises. Long sessions and physical haptic response remain less tested.

No claim of official PortMaster catalogue acceptance. Packaging follows https://portmaster.games/packaging.html. Alto's Adventure belongs to its creators; this project is unaffiliated.

# Validation — RG35XX-H / KNULLI, 2026-10-14

- APK 1.8.8 SHA-256: `efb8eb2054e6c66173fc1638e6d005f048b48446b6029c4fdcb8aa8b9dcaaae0`.
- Built both ARM64 loader and OpenSL shared bridge from this source using Zig 0.13.0. The one-command package build completed and produced `altosadventure.zip` with no APK, game libraries or system GPU drivers.
- Fresh installation into an isolated system root from the original APK: exit 0. Repeated installation: exit 0. Missing or incorrect APK: rejected with exit 1 and an actionable message. A sentinel preference file survived reinstallation.
- Final launcher runs install/update automatically. Existing real assets matched the APK; save directories were preserved. The experimental entry was replaced by Alto's Adventure, preserving game statistics, and a vertical 654×1024 2D box-art cover was attached to EmulationStation metadata.
- Final title/audio check uses an external timeout; timeout exit 124 is intentional and is not a graceful save test. The final package reached Title (state 2), with nonzero Pulse PCM and zero recorded underruns, using only Alto and system library paths. Earlier tests verified graceful quit and save/reload. Source checks and JSON/XML/shell parsing are part of the local validation.
- Earlier controller verification covered sliders, Settings tabs and modal controls. Offline policy has 45 guarded methods. Gameplay pacing comparison with separate copied profiles completed both 1500-frame runs with exit 0: old 28.36/28.98 FPS, new 47.15/50.03 FPS. These short runs use different scenes and do not establish minimum sustained FPS.
- Physical haptic response, unusually long runs and firmware/device combinations other than this KNULLI RG35XX-H remain less tested.

Private logs, APK, original libraries, covers and screenshots are retained outside Git. Package structure follows https://portmaster.games/packaging.html; this is not an official catalogue submission.

Cover correction: recovered the established vertical 2D box-art convention from the other project conversation. LaunchBox Android cover 654×1024 replaces the horizontal screenshot; screenshot.png remains 640×480. The package builder now rejects horizontal covers before compilation.

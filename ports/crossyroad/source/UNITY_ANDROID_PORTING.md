# Reusing the Crossy Road work for other Android Unity games

This is a field guide from the working Crossy Road 4.5.0 port on KNULLI RG35XX-H. It is a starting point for investigation, **not** a universal loader. The working code contains Crossy Road and Unity-version-specific addresses, JNI responses, UI methods, and save hooks. Do not run its preload against another game unchanged.

## What proved reusable

1. **Inventory the APK and pin the version.** Record its SHA-256, ABI (`arm64-v8a` is required on this device), Unity version, IL2CPP versus Mono, `libunity.so`, `libil2cpp.so`, metadata, assets, and native dependencies. Keep the unmodified APK and extracted files. An APK update can invalidate native addresses and patches.
2. **Stage assets and libraries separately from saves.** For this port, `/userdata/system/cr` contains the game files and `/userdata/system/cr/userdata` contains its save. The executable and native bridge live under `/userdata/system/crossyroad-lab`. The user data directory must survive every port update.
3. **Use one SDL controller path.** The loader opens the controller once and sends SDL events to Android/Unity. Physical button indices must be measured on the actual handheld; SDL indices differ from Linux `jstest` indices. On this RG35XX-H, physical B/Y/Start/F mapped to SDL semantics A/B/Start/Guide. SDL axes 0/1 from the left stick are translated into D-pad press and release events after a deadzone. Avoid reading `/dev/input` in parallel because it creates duplicate or inconsistent actions.
4. **Fill only the Android services a game actually uses.** A Unity Android build expects JNI classes, input devices, timing, storage, audio, and sometimes billing or analytics. Instrument failures and emulate the narrow behavior required by that build. Crossy Road needed a correct `KeyEvent`→`InputEvent` type relation and `InputDevice` enumeration before Unity accepted the controller.
5. **Treat save writes as a separate gate.** Crossy Road queued a background save write that its research loader never ran. The bridge forces the encrypted save write synchronously, uses a stable local device identifier, and calls the game's save method on F+Start. A successful UI score display alone did not prove persistence; a save file and a successful reload did.
6. **Keep audio and performance independent.** Crossy Road needed an OpenSL path for sound. Test audio, input, rendering, frame pacing, and long-session memory separately; a fix to one can regress another.

## What must be redone for each game

- Native function addresses, expected AArch64 opcodes, Unity layout, and `GameController` or equivalent UI methods.
- Java/JNI class and method behavior, including optional services. A generic fake object caused managed exceptions in Crossy Road.
- Save schema, background worker behavior, encryption/device ID, and file paths.
- Audio backend and graphics API requirements.
- UI focus and touch-only screens. A game can support controller movement but still need game-specific menu routing.

The current pinned executable came from a research loader. Recompiling the broader source did not reproduce its behavior; one attempted rebuild crashed at startup. The release uses a **single verified opcode patch** to that pinned binary and a rebuilt game-specific preload. `source/patch_loader_sdl.py` validates the original hash and instruction before writing the executable. `source/build_native.sh` builds the preload; its output must be checked against the pinned package hash before use. Those verification checks are essential when adapting the process.

## Recommended clean workflow for the next game

1. Run `audit_apk.sh` on the candidate APK and record its hash, Unity build, ABI, and native libraries. Check available RAM and screen resolution against the game's needs before porting.
2. Keep a read-only original and a separate working directory. Extract assets without modifying the APK.
3. Build a minimal launch that reaches the title screen. Log startup failures and identify the precise missing JNI or native dependency.
4. Make SDL controller detection work; record SDL events for the device. Add game-specific menu callbacks only where native UI input is insufficient.
5. Add audio and save persistence. Verify saving by exiting and relaunching, including a score or unlock that changes.
6. Run a longer play session, then return to EmulationStation. Check logs, memory, audio, controls, and save files after the session.
7. Freeze the working executable, source, hashes, and launcher in a small `release/` directory. Archive experiments elsewhere and preserve rollback copies.

This public repository contains the source and local packaging recipe. The pinned loader, device libraries, game assets, and research archive remain outside Git. The currently working package cannot be rebuilt from this source alone.

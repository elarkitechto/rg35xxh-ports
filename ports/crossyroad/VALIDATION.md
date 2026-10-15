# Crossy Road candidate validation

## Pinned game

Android APK4.5.0 SHA-256: 1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb. Game libraries are verified against crossy-libraries.sha256 and extracted from the user's APK; never bundled.

## Reconstructed loader

The earlier package depended on a pinned research executable. The candidate instead compiles runtime/main_crossy.c and the original generic ELF/JNI/SDL framework, game-specific preload and OpenSL bridge in a single host build. The preload discovers the JNI table and FindClass function through stable exported loader APIs; fixed fallback addresses exist only for compatibility with the older installed executable. Native key-event lookup likewise uses an exported symbol. No private loader binary is a build input.

The new bootstrap omits Hollow Knight-specific code offsets, enables the Android pthread compatibility shim, waits for game bridge installation before JNI_OnLoad, loads OpenSL as one shared implementation (avoiding duplicate interface IDs), and rejects a null address from Android network enumeration before inet_ntop. GL spoofing/diagnostic interception is bypassed on the tested H700 native GLES driver. A 700-frame isolated run reached rendered game scenes and nonzero Pulse PCM with no recorded underruns and exited0. This initial run alone did not establish save persistence or full controller behavior.

## Touch pause HUD

Original GameController.Update at RVA0x150cf80 passes `and w1,w8,#1` (0x12000101) to pauseButton.gameObject.SetActive. The guarded replacement `mov w1,wzr` (0x2a1f03e1) keeps only that icon hidden. Start retains GameController.ButtonPause / PauseController.Unpause. Native visual controls are not replaced by label matching.

An isolated 1200-frame diagnostic exercised the same queued action used by SDL Start: `paused=1 PASS`, then `resumed=1 icon-active=0 active=1 started=1 PASS`. Guarded opcode installation succeeded. A 640×480 frame capture was visually inspected; the pause icon was absent. This tests the action path, not a fresh physical button press.

The diagnostic performed a synchronous encrypted save at exit and returned0; the save structure retained the copied profile’s top score218. Stable data was not used for these writes. A separate400-frame reload read the saved profile and synchronously saved top score218 again, exiting0. Checksums of the stable profile preferences remained identical. Historical stable feature tests are documented in source/UNITY_ANDROID_PORTING.md and must not be presented as fresh physical RC tests.


## Installer and PortMaster integration

On RG35XX-H / KNULLI Scarab, using the installed PortMaster2026.06.23-0015 control.txt and funcs.txt:

- A fresh isolated package install extracted and verified the exact APK and game libraries through the top-level launcher; no manual install.sh invocation was required for first launch.
- The900-frame initial launch rendered, exercised pause/resume with the icon hidden, saved and returned0. Nonzero Pulse PCM with no logged underruns confirms generated audio; no new listening or physical haptic test is claimed.
- Repeated installer execution preserved a sentinel in existing prefs. Missing and incorrect APKs were rejected in a separate empty destination.
- Integration testing exposed preload inheritance into PortMaster cleanup helpers. The preload is now scoped to the game process only. Fixture coverage checks that helper/cleanup do not receive it.
- A direct SSH launch lacked EmulationStation’s temporary controller database. The runtime now registers the verified RG35XX-H GUID only when SDL has no mapping; supplied PortMaster mappings take priority. The corrected700-frame package retest opened a non-null SDL controller, passed pause/resume, saved, exited0 and completed cleanup without the earlier helper crashes. The same negative APK/preservation checks passed again.
- Eight launcher fixture tests pass (controls/paths/diagnostics, fresh defaults, CFW mods, exit status, setup failure, architecture rejection, signal cleanup, missing PortMaster). Alto’s eight existing fixtures also pass.

For bounded tests only, a copy of the launcher changed HK_FRAMES and enabled diagnostics. The distributed launcher uses HK_FRAMES=0 and clears test flags. EmulationStation was restored after each graphical test.

## Remaining testing

Physical button-by-button menus, listening/haptics and20–30 minute endurance sessions still need a fresh pass with this reconstructed loader. Stable predecessor results do not substitute for that pass. Other devices, CFWs and resolutions remain untested; proprietary Mali linkage is a known portability limit. This is a testing candidate, not a catalogue acceptance claim.


## Existing handheld installation

The installed stable executable was retained. Only its game bridge and matching installer checksum were updated, with rollback copies kept privately on-device. A400-frame bounded smoke launch with the older executable confirmed the guarded pause-icon patch and exited0. No scripted gameplay/save migration was performed there; normal launch may update usage preferences. The existing vertical cover matched the candidate’s SHA-256, and EmulationStation was restored. The reconstructed candidate remains a separate isolated test installation.

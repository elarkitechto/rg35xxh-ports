# Crossy Road 0.2.0-rc1 — PortMaster testing candidate

Thanks to Hipster Whale for creating Crossy Road and to the upstream Android native loader authors credited in licenses/NOTICE.txt. Porter: elarkitechto. This is a community testing build, not an accepted PortMaster release.

## Install — one handheld entry point

Extract the ZIP into Ports, copy your original `crossy-road-4-5-0.apk` into `crossyroad/`, and open **Crossy Road**. The launcher loads PortMaster configuration, verifies/extracts the APK and starts the game automatically. Do not execute install-data manually. No second launcher or compiler is needed on the handheld. No APK/assets/GPU driver is included or downloaded.

Required Android ARM64 APK 4.5.0 SHA-256: `1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb`. Other APK versions do not work with these guarded addresses.

## Scope

| Configuration | Evidence |
| --- | --- |
| RG35XX-H / KNULLI Scarab / 640×480 / Mali | Stable predecessor physically verified for controls, audio, local saving and haptics; reconstructed loader/RC retests recorded in VALIDATION.md |
| Other H700/KNULLI devices | Not tested |
| muOS / AmberELEC / dArkOS / ROCKNIX | Not tested; backend dependencies require review |
| Panfrost-only / other GPU drivers | Not supported as-is: current build links proprietary libmali.so.0 |
| 720×720 / higher resolutions | Not tested |
| ARM32 / x86 | Unsupported by this ARM64 candidate |

This candidate labels experimental=true and makes no broad CFW promise. SDL2, zlib, Mali GLES and PulseAudio client libraries come from firmware. Package binaries are compiled from the published source; the historical pinned research executable is no longer a build input.

## Controls / behavior

PortMaster supplies the SDL mapping. If its temporary database is absent, the runtime adds only the verified RG35XX-H GUID mapping when SDL has none; it preserves existing mappings. On the tested RG35XX-H: D-pad/left stick move; B accepts/advances; Y goes back; Start pauses/resumes; F+Start saves/quits. Touch pause icon is hidden without disabling the original pause action. Menus, gifts, character selection, audio and local progress retain the stable bridge's behavior. Haptics cover jump, death and selected environment events; verify physical behavior per device. Standard exit-hotkey differences on other CFWs need review.

Fresh installs keep `crossyroad/gamedata/` and `crossyroad/runtime/`. Existing KNULLI saves in `/userdata/system/cr` are reused in place. No automatic save migration or overwrite. Logs: `crossyroad/install.log`, `crossyroad/log.txt`, `runtime-dependencies.log` if libraries are missing. Back up saves before testing.

## Test/report checklist

Report device, CFW build, GPU backend, resolution, RC version, mapping differences, reproduction steps and relevant log excerpts. Do not upload APK/assets or private saves. Check fresh setup; repeated launch; missing/wrong APK; gameplay in four directions; pause/resume without touch icon; audio; gifts/prize machine; character selection; menu sliders; haptics; graceful quit/reload; 20–30 minute sessions. Mark untested items pending.

Before an upstream PR, open a thread in Discord #testing-n-dev, collect the required CFW/resolution results and maintainer feedback, and link that history in the PR. Pinned APK availability and licenses require maintainer review. No Discord post or upstream PR has been sent automatically.

# Alto's Adventure 0.2.0-rc1 — community testing candidate

This is a testing build, not an accepted PortMaster catalogue release. Thanks to Team Alto / Snowman for creating Alto's Adventure and Noodlecake for its Android release. Thanks to the upstream Android-native port framework authors credited in licenses/NOTICE.txt. Porter: elarkitechto.

## Install on the handheld — one entry point

1. Extract the ZIP into Ports, keeping `Alto's Adventure.sh` beside `altosadventure/`.
2. Supply your own Android Alto **1.8.8 ARM64** APK as `altosadventure/altos-adventure-1-8-8.apk`.
3. Open **Alto's Adventure**. The launcher loads PortMaster controls, verifies the APK, automatically extracts/prepares everything and starts the game. Do not run install-data manually. No compiler is required on the handheld.

APK SHA-256: `efb8eb2054e6c66173fc1638e6d005f048b48446b6029c4fdcb8aa8b9dcaaae0`. APK/game assets are not included or downloaded. Other APK versions are not compatible. Contact the porter if that exact version is not available to you; do not submit logs or archives containing the APK or extracted game data.

## Compatibility — evidence, not promises

| Device / CFW / resolution | Status |
| --- | --- |
| RG35XX-H / KNULLI / 640×480, proprietary Mali SDL backend | Previously verified gameplay, audio, local saves and menu controls; RC integration retest recorded in VALIDATION.md |
| Other H700 devices / KNULLI | Not tested |
| muOS / AmberELEC / dArkOS | Not tested; dependencies/backend may need adaptation |
| ROCKNIX libmali / Panfrost / other GPU drivers | Not tested; this build links libmali.so.0 and does not support a Panfrost-only system as-is |
| Other resolutions, including 720×720 | Not tested; UI scaling must be checked |
| ARM32, x86 | Unsupported by this ARM64 candidate |

Only ARM64 is declared in port.json. Experimental=true intentionally keeps this labelled as testing. SDL2, zlib, proprietary Mali GLES libraries and PulseAudio client libraries come from the firmware. We do not bundle a GPU driver. A broad PortMaster compatibility claim requires community results and may require additional backend work.

## Controls and offline scope

Controls are read from PortMaster's SDL controller mapping. In the tested RG35XX-H mapping B confirms/jumps, Y cancels, D-pad/left stick navigates, L/R changes menu tabs, left/right adjusts selected sliders, Start pauses/resumes, F+Start saves and quits. Report any mapping differences on other CFWs. Long-press jump supports backflips. The touch pause icon is hidden.

Local coin-funded upgrades/revival remain enabled. Ads, Google Play sign-in, billing and telemetry are disabled. Share and rewarded-video controls are hidden. The post-upgrade single-button Thanks modal closes with confirm or back. Physical vibration is implemented but needs more community validation.

## Data, updates and reports

Fresh installs use `altosadventure/gamedata/` (assets, prefs, userdata) and `altosadventure/runtime/`. An existing KNULLI installation with `/userdata/system/alto/prefs` continues using `/userdata/system/alto` automatically; saves are not migrated or overwritten. Logs: `altosadventure/install.log`, `altosadventure/log.txt`, and `runtime-dependencies.log` if applicable. Back up your prefs before testing.

Please report: RC version, device, CFW/build, GPU backend, resolution, controls/mapping issues, exact reproduction steps and relevant log excerpts. Do not upload game data. Test a fresh install and a repeat launch; sound/music sliders; every menu/tab; local save after graceful quit/reopen; coin upgrade and Thanks; revival with earned coins; pause/resume; missing/wrong APK message; a 20–30 minute session; haptics if available. Mark tests not performed as pending.

## Before an upstream PR

Create a thread in PortMaster Discord #testing-n-dev, attach the testing package and source/build links, collect the required CFW/resolution results, fix feedback and link that thread in the PR. Maintainers must review the pinned Android APK requirement, source/asset licensing and supported backend scope. No Discord message or PR has been posted automatically.

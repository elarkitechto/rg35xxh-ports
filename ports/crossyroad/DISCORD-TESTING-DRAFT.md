# Draft — not posted

**Crossy Road Android native ARM64 — testing candidate 0.2.0-rc1**

I would like to submit this adaptation for community testing. The stable predecessor has been played on RG35XX-H/KNULLI at640×480 with native SDL controller input, menus, audio, persistent local saves and haptics. This candidate rebuilds the loader, JNI/input preload and OpenSL bridge from published source; it no longer requires the pinned research executable.

Users supply their own exact Android4.5.0 ARM64 APK (SHA-256 in TESTING.md). First launch verifies/prepares the APK and starts the game automatically; no second handheld script or compiler. APK/assets and proprietary GPU drivers are not bundled. Is the pinned APK requirement suitable for a catalogue proposal?

Launcher integrates PortMaster control.txt, CFW mods, controller mapping, platform helper and cleanup. Fresh data/runtime are self-contained; existing KNULLI saves are reused. Touch pause icon is removed while Start continues to pause/resume. Please see VALIDATION.md for the exact RC evidence and pending tests.

Current support evidence is RG35XX-H/KNULLI only. This build links libmali.so.0; Panfrost-only and other CFW/backend configurations are not supported as-is. Help testing devices/CFWs, resolutions, mapping/hotkeys, long sessions and haptics would be appreciated.

Source/build guide: https://github.com/elarkitechto/rg35xxh-ports/tree/port/crossyroad-rc/ports/crossyroad

Attach the reviewed crossyroad-0.2.0-rc1.zip and checksum after checking artwork distribution permissions. Never attach the APK, extracted game libraries or private saves. This draft and local testing package have not been posted to Discord or an upstream PR.

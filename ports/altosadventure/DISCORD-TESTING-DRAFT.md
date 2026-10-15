# Draft for #testing-n-dev — not yet posted

**Alto's Adventure — Android native ARM64 testing candidate 0.2.0-rc1**

I have an offline controller adaptation of Android Alto's Adventure 1.8.8 using an ARM64 Unity/IL2CPP compatibility layer. Gameplay, audio, local save/reload, NGUI menu controls and installation have been tested on RG35XX-H/KNULLI at 640×480. I would like feedback on suitability for PortMaster and help testing other CFWs/devices.

The package includes the compiled loader/audio bridge and source/build instructions, but no APK, extracted game assets or proprietary GPU driver. Users supply their own exact 1.8.8 ARM64 APK; first launch validates its SHA-256 and installs everything automatically. No separate handheld installer step or compilation is needed. Is that pinned APK requirement acceptable for a catalogue submission?

PortMaster control.txt, CFW mods, controller mapping and cleanup are integrated. Fresh data lives inside the port directory, with compatibility for existing KNULLI saves. Google Play, ads, telemetry and billing are disabled; coin-funded upgrades/revival and local progress remain. Share, rewarded-video controls and touch pause icon are hidden.

**Current limitation:** tested only RG35XX-H/KNULLI. The build links libmali.so.0 and uses the tested KNULLI Mali SDL backend; Panfrost-only systems are not supported as-is. No claim of all-CFW compatibility. The exact APK is required; other versions are untested. Extended sessions, vibration and other resolutions need more testing.

**Source:** https://github.com/elarkitechto/rg35xxh-ports/tree/port/altos-adventure/ports/altosadventure

**Build:** tools/build-alto-local.sh in the same branch. Tester instructions and checklist: TESTING.md. Upstream framework/license attribution: NOTICE and LICENSE.

**Package attachment:** attach the reviewed altosadventure-0.2.0-rc1.zip and its SHA-256 file here after checking artwork distribution permissions. Please do not attach any APK or game data. This local build has not been uploaded to a public release.

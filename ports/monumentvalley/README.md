# Monument Valley

Android port for **RG35XX-H with KNULLI and PortMaster**. Supply your own
**Monument Valley 3.10.114 ARM64 APK**. Game files are not included.

## Install

1. Download [monumentvalley.zip](https://github.com/elarkitechto/rg35xxh-ports/releases/download/monumentvalley-v0.1.1/monumentvalley.zip) and extract it into **Ports**.
2. Rename the compatible APK to **monument-valley-3.10.114.apk** and copy it into **ports/monumentvalley/**.
3. Open **Monument Valley** from Ports. First-run conversion is automatic; no compiler or separate setup launcher is needed. Keep the APK in that folder.

**Required APK SHA-256:**
```text
6a786c9cc65a196abb45e29dd01c400dfcf547d42fe004e3b084f235ac5fa301
```

The exact hash is required: the compatibility profile targets this ARM64 build.
Other repacks, Netflix editions and Windows/Panoramic files are unsupported.
This is the Android game with its original landscape layout, not the PC edition.

## Controls

| Control | Action |
| --- | --- |
| Left stick / D-pad | Move the cursor or navigate menus |
| Right stick in TATE | Move the cursor; left stick remains available |
| A, B, X, Y | Follow the game's prompts: physical letters match game letters |
| L2 + R2 | Toggle landscape / TATE |
| F + Start | Save and exit to Ports |

The game starts in landscape. TATE changes the logical resolution to portrait
and rotates the output; the original game handles its layouts and interactions.

## Haptic feedback

Short pulses accompany accepted menu actions, pressure-button down animations,
conversation starts and level completion. Authored explosion and collapse
sequences pulse while their original coroutine runs, stopping on completion,
cancellation or scene teardown. Other animations and camera pans stay silent.
Feedback uses the controller's rumble API and adds no render/audio delays.
Set `MONUMENT_HAPTICS=0` before launching to disable it.

## Saves and updates

Saves remain in `/userdata/system/monument-valley/saves/`. Keep that folder
when updating; replace the package files and keep your APK. The installer never
seeds or replaces saves. Online services are unavailable.

## Troubleshooting

For setup failures, check the exact APK hash and `monumentvalley/install.log`.
For runtime problems, check `monumentvalley/log.txt`. Some asset loads can still
stutter. Compatibility is limited to RG35XX-H/KNULLI; complete game coverage
has not been verified.

## Credits

Monument Valley: **ustwo games**. Compatibility framework: **felc18-blip /
initdream**, with upstream credits retained in NOTICE. Port: **elarkitechto**.
Cover artwork belongs to ustwo games and depicts Monument Valley; its Panoramic
Edition branding does not change which executable this package runs.

[Licenses and attribution](https://github.com/elarkitechto/rg35xxh-ports/blob/main/ATTRIBUTION.md)

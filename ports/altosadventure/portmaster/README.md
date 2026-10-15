# Alto's Adventure

## Installation

Copy the Android **Alto's Adventure 1.8.8** APK to `ports/altosadventure/` and name it `altos-adventure-1-8-8.apk`. Launch **Alto's Adventure** from Ports. The first launch verifies the APK and prepares the game files automatically. Keep the APK in place for future reinstalls.

The supported APK has SHA-256 `efb8eb2054e6c66173fc1638e6d005f048b48446b6029c4fdcb8aa8b9dcaaae0`. Other releases have not been tested. If installation fails, check `ports/altosadventure/install.log`.

## Controls

| Button | Action |
| --- | --- |
| D-pad | Navigate menus |
| B | Select / jump; hold for a backflip |
| Y | Back / cancel |
| Start | Pause / resume |
| L / R | Change Settings page |
| F + Start | Save and quit |

Left/right adjusts the selected Settings slider. Progress is saved locally. Online services, ads and purchases are unavailable in this port.

## Notes

Tested on RG35XX-H with KNULLI at 640×480. Thanks to the authors of the Android compatibility framework credited in `NOTICE`. Port adaptation and packaging by [elarkitechto](https://github.com/elarkitechto). Alto's Adventure belongs to Snowman; this is an unofficial port.

Thanks to Team Alto / Snowman for creating the game and Noodlecake for its Android release. Framework authors are credited in licenses/NOTICE.txt.

Fresh installs store data in `altosadventure/gamedata/` and runtime in `altosadventure/runtime/`. Existing KNULLI saves under `/userdata/system/alto` are reused automatically. The launcher uses PortMaster control.txt, get_controls, CFW mods and cleanup. See the included TESTING.md for the compatibility matrix, dependency limits and test checklist. Logs are inside `altosadventure/`. Only RG35XX-H/KNULLI is verified; this candidate is not an official catalogue release.

# Threes!

Android port for **RG35XX-H running KNULLI**, with controller support and local
saves. Requires PortMaster and your **Threes! full 3.1.6144 ARM64** game file.

## How to install


**Required game file:** `threes-3.1.6144.apk`

**SHA-256:**
```text
7d28059f0993285382da108324f844d146b375b64a1600fac4b8e6f5cf3c482a
```

1. Download [threes.zip](https://github.com/elarkitechto/rg35xxh-ports/releases/latest/download/threes.zip) and extract it into **Ports**.
2. Copy `threes-3.1.6144.apk` into `ports/threes/`.
3. Launch **Threes!** from Ports. First-run setup is automatic.

Keep the APK in that folder. When updating, replace the package files and keep your saves.

## Controls

| Button | Action |
| --- | --- |
| D-pad / either stick | Move tiles; navigate menus |
| A / B in menus | Confirm / back |
| L1 / R1 or left / right in menus | Switch side screens |
| Up / down in menus | Select an option |
| Start / Select during play | Menu / statistics |
| L2 + R2 | Toggle TATE / landscape |
| F + Start or Select + Start | Save and exit |

The game starts in landscape. Press L2 + R2 to toggle TATE, then rotate the
handheld 90° clockwise. Face buttons also move tiles during TATE gameplay;
menus keep A/B as confirm/back. Landscape
preserves the board’s proportions and extends the background to fill the screen.
Score signing uses the default name.

This package uses the full edition; the Free APK/XAPK is not
compatible with this runtime. Online services are unavailable.

Saves: `/userdata/system/threes-android/saves/`.

## Saves and updates

Keep the game file and your save folder when updating. Replace the port package
files, then launch the game normally. Game data is prepared automatically.

## Troubleshooting

Check the game-file name and version, then read `threes/install.log` or
`threes/log.txt`. Compatibility has only been tested on RG35XX-H with KNULLI.



[Credits and licenses](../../ATTRIBUTION.md)

Thanks to Sirvo for creating the original game.

# Crossy Road

Android port for **RG35XX-H running KNULLI**, with controller support and local
saves. Requires PortMaster and your **Crossy Road 4.5.0 ARM64** game file.

## How to install


**Required game file:** `crossy-road-4-5-0.apk`

**SHA-256:**
```text
1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb
```

1. Download [crossyroad.zip](https://github.com/elarkitechto/rg35xxh-ports/releases/latest/download/crossyroad.zip) and extract it into **Ports**.
2. Copy `crossy-road-4-5-0.apk` into `ports/crossyroad/`.
3. Launch **Crossy Road** from Ports. First-run setup is automatic.

Keep the APK in that folder. When updating, replace the package files and keep your saves.

## Controls

| Button | Action |
| --- | --- |
| D-pad / left stick | Move / navigate |
| B | Confirm / continue |
| Y | Back |
| Start | Pause / resume |
| F + Start | Save and exit |

Online services and ads are unavailable. Progress is saved locally, using
existing saves in `/userdata/system/cr/`.

## Saves and updates

Keep the game file and your save folder when updating. Replace the port package
files, then launch the game normally. Game data is prepared automatically.

## Troubleshooting

Check the game-file name and version, then read `crossyroad/install.log` or
`crossyroad/log.txt`. Compatibility has only been tested on RG35XX-H with KNULLI.



[Credits and licenses](../../ATTRIBUTION.md)

Thanks to Hipster Whale for creating the original game.

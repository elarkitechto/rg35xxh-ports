# Alto’s Adventure

Android port for **RG35XX-H running KNULLI**, with controller support and local
saves. Requires PortMaster and your **Alto’s Adventure 1.8.8** game file.

## How to install


**Required game file:** `altos-adventure-1-8-8.apk`

**SHA-256:**
```text
efb8eb2054e6c66173fc1638e6d005f048b48446b6029c4fdcb8aa8b9dcaaae0
```

1. Download [altosadventure.zip](https://github.com/elarkitechto/rg35xxh-ports/releases/latest/download/altosadventure.zip) and extract it into **Ports**.
2. Copy `altos-adventure-1-8-8.apk` into `ports/altosadventure/`.
3. Launch **Alto’s Adventure** from Ports. First-run setup is automatic.

Keep the APK in that folder. When updating, replace the package files and keep your saves.

## Controls

| Button | Action |
| --- | --- |
| D-pad | Navigate |
| B | Select / jump; hold for a backflip |
| Y | Back / cancel |
| Start | Pause / resume |
| L1 / R1 | Switch settings pages |
| F + Start | Save and exit |

In Settings, left/right adjusts the selected slider. Online services, ads and
purchases are unavailable. Existing saves use `/userdata/system/alto/`.

## Saves and updates

Keep the game file and your save folder when updating. Replace the port package
files, then launch the game normally. Game data is prepared automatically.

## Troubleshooting

Check the game-file name and version, then read `altosadventure/install.log` or
`altosadventure/log.txt`. Compatibility has only been tested on RG35XX-H with KNULLI.



[Credits and licenses](../../ATTRIBUTION.md)

Thanks to Snowman and Team Alto for creating the original game.

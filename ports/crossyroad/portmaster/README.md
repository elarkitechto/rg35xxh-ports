# Crossy Road

Android port for **RG35XX-H running KNULLI**, with controller support and local
saves. Requires PortMaster and your **Crossy Road 4.5.0 ARM64** game file.

## Install

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

## Troubleshooting

Check the game-file name and version, then read `crossyroad/install.log` or
`crossyroad/log.txt`. Compatibility has only been tested on RG35XX-H with KNULLI.

<details>
<summary>Supported game file SHA-256</summary>

`1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb`

</details>

Credits and licenses: https://github.com/elarkitechto/rg35xxh-ports/blob/main/ATTRIBUTION.md

Thanks to Hipster Whale for creating the original game.

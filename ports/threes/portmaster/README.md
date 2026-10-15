# Threes!

Android port for **RG35XX-H running KNULLI**, with controller support and local
saves. Requires PortMaster and your **Threes! Free 3.1 (24576), including the base APK and ARM64 split** game file.

## Install

1. Extract the port ZIP into **Ports**.
2. Copy `threes-free-3-1-24576.xapk` into `ports/threes/`.
3. Launch **Threes!** from Ports. First-run setup is automatic.

When updating, replace the package files and keep your saves.

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

The game starts in TATE: rotate the handheld 90° clockwise. Face buttons also
move tiles during TATE gameplay; menus keep A/B as confirm/back. Landscape
preserves the board’s proportions and extends the background to fill the screen.
Score signing uses the default name.

Night mode keeps the original night tiles. Threes! and the Triples demake use
separate saves. The Free version retains its original play restrictions; videos,
online services and purchases are unavailable and are not unlocked by this port.

Saves: `/userdata/system/threes-android/saves/`.

## Troubleshooting

Check the game-file name and version, then read `threes/install.log` or
`threes/log.txt`. Compatibility has only been tested on RG35XX-H with KNULLI.

<details>
<summary>Supported game file SHA-256</summary>

`99dcba5f8dbcbc4294507c6b30d9f0719ae4127228186e9a471ecdb45a77c880`

</details>

Thanks to Sirvo for creating the original game.

Source and build instructions: https://github.com/elarkitechto/rg35xxh-ports/tree/main/ports/threes

# Katana ZERO

Port for **RG35XX-H with KNULLI and PortMaster**. Requires your own
**Katana ZERO Netflix 1.1.10 ARM64 XAPK**. Game files are not included.

## How to install

1. Download **katanazero.zip** from this project's [Releases](https://github.com/elarkitechto/rg35xxh-ports/releases).
2. Extract it into your handheld's **Ports** folder.
3. Rename your **Katana ZERO Netflix 1.1.10 ARM64 XAPK** to
   **katana-zero-1.1.10.xapk** and copy it into **ports/katanazero/**.
4. Open **Katana ZERO** from Ports. First-time setup runs automatically; wait
   for it to finish. No compiler or second setup script is needed.

After copying, the files should be arranged like this:

```text
ports/
├── Katana ZERO.sh
└── katanazero/
    ├── katana-zero-1.1.10.xapk   ← your game file
    └── …                       ← files supplied by the port ZIP
```

Other game versions and Windows/Switch files are not supported by this installer.

## Controls

Use the D-pad/sticks and the game's controller button prompts.
**F + Start** exits to Ports. Touch controls are hidden automatically.

## Saves and updates

Keep **ports/katanazero/save/** when updating. Replace the port files without
removing that folder. Progress is saved locally; Netflix online/cloud features
are unavailable.

## Troubleshooting

- **Setup fails:** check the XAPK version, filename and folder, then see `katanazero/install.log`.
- **The game closes or has audio problems:** see `katanazero/log.txt`. This is an experimental port; audio warnings and an exit crash remain under investigation.
- **Performance drops:** try disabling Bloom in the game's graphics settings.

## Credits

Game: **Askiisoft / Devolver Digital**. Android distribution: **Netflix**.
Loader: [GMLoader-next](https://github.com/JohnnyonFlame/gmloader-next) by
JohnnyonFlame and contributors. Port: **elarkitechto**.

[Licenses and attribution](https://github.com/elarkitechto/rg35xxh-ports/blob/main/ATTRIBUTION.md)

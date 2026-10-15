# PortMaster candidates

| Port | Directory / package | Status |
| --- | --- | --- |
| Alto’s Adventure | `altosadventure` / `altosadventure.zip` | Testing candidate |
| Crossy Road | `crossyroad` / `crossyroad.zip` | Testing candidate |
| Threes! original | `threes` / `threes.zip` | Testing candidate |
| Triples demake | `ports/triples` | In development; excluded |

Follow the [build instructions](BUILD.md) to create each package. The build
command compiles and packages the complete runtime. The player supplies the
compatible APK or XAPK and opens the launcher; data setup runs automatically.

Prepare the submission tree from the repository root:

```sh
python3 tools/prepare-portmaster.py altosadventure
python3 tools/prepare-portmaster.py crossyroad
python3 tools/prepare-portmaster.py threes
```

Output: `.local/portmaster/port/<name>/`. Each candidate includes metadata,
README, screenshot, cover, launcher, runtime and licenses. The preparation tool
checks required files and rejects APKs, XAPKs and saves. Use `--output` for a new
revision; existing output directories are not overwritten.

The candidates follow the [PortMaster package structure](https://portmaster.games/packaging.html).
Before submitting, run the official PortMaster-New checks on the staged tree
and complete compatibility and licensing review. Testing currently covers only
RG35XX-H with KNULLI and uses the device’s graphics libraries. These ports are
not yet included in the official PortMaster catalog.

## Launcher conventions

The launchers follow PortMaster's bootstrap: locate `control.txt`, load the CFW
mod, call `get_controls`, set game paths and SDL mapping, call
`pm_platform_helper`, run the binary and finish with `pm_finish`.
References: [Defblade](https://github.com/PortsMaster/PortMaster-New/blob/main/ports/defblade/Defblade.sh),
[Celeste](https://github.com/PortsMaster/PortMaster-New/blob/main/ports/celeste/Celeste.sh),
and the [official packaging guide](https://portmaster.games/packaging.html).

Automatic first-run preparation is part of PortMaster's documented pattern.
These ports verify the owner's exact APK and extracted Unity libraries because
the native patches are version-specific. Normal launches reuse the installed
data. Binaries are bundled once in `runtime/`; startup does not copy them from
a second location or run `ldd`. Runtime output goes to the per-launch `log.txt`;
the loader no longer appends a second `debug.log`.

Every build checks that the packaged launcher and preparer match the current
source and rejects owner files, logs and development leftovers. To validate a
previously built archive without extracting it:

```sh
python3 tools/prepare-portmaster.py threes --check-only
```

# Development

Players should start with the [download and installation guide](README.md).
Each port's `portmaster/` folder is the package template; `source/` contains the
implementation and build instructions. Compiled player packages belong in Releases.

- [Alto’s Adventure](ports/altosadventure/source/)
- [Crossy Road](ports/crossyroad/source/)
- [Threes!](ports/threes/source/)
- [Katana ZERO build](ports/katanazero/source/README.md)
- [Katana ZERO validation and release gate](ports/katanazero/source/COMPATIBILITY.md)
- [Monument Valley](ports/monumentvalley/source/README.md)
- [Triples demake source](ports/triples/) — no player release

## PortMaster conventions

Player instructions use download/install, required game files, target folder,
first launch, controls, saves and troubleshooting. Package metadata carries the
same installation steps in `attr.inst` and `attr.inst_md`. Do not claim official
catalog availability or link an unpublished ZIP.

References: [PortMaster porting documentation](https://portmaster.games/porting.html)
and the [official Celeste package](https://github.com/PortsMaster/PortMaster-New/tree/main/ports/celeste).

Retain corresponding source and third-party licenses for binary releases. Public
packages must contain no APK/XAPK, extracted proprietary game runtime, seeded save,
credentials or development logs.

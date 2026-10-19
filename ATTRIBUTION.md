# Attribution

## Android compatibility framework

The ports reuse the ARM64 Android loader and shims from
[nextos_ports_android](https://github.com/initdream/nextos_ports_android), with
credit retained to felc18-blip / initdream. Its upstream NOTICE also credits
mtojek’s [syberia_arm64](https://github.com/mtojek/syberia_arm64) and
[lswtcs_arm64](https://github.com/mtojek/lswtcs_arm64).

The framework and original adaptations in this repository use Apache-2.0.
Upstream license notices are retained in LICENSE and NOTICE files. Porting,
controller support, save integration, installers and packaging are by
[elarkitechto](https://github.com/elarkitechto).

## Games

- **Alto’s Adventure** and its assets belong to Snowman / Team Alto. Framework notices: [LICENSE](ports/altosadventure/LICENSE), [NOTICE](ports/altosadventure/NOTICE).
- **Crossy Road** and its assets belong to [Hipster Whale](https://www.crossyroad.com/). Its JNI bridge includes game-specific adaptations. Framework notices: [LICENSE](LICENSE), [NOTICE](NOTICE).
- **Threes!** and its assets belong to Sirvo. This port reuses the framework and Alto/Crossy adaptations. Notices: [LICENSE](ports/threes/LICENSE), [NOTICE](ports/threes/NOTICE).

- **Monument Valley** and its assets belong to ustwo games. Compatibility framework notices: [LICENSE](ports/monumentvalley/LICENSE), [NOTICE](ports/monumentvalley/NOTICE).

Players supply their own compatible game files. The repository license does not
grant rights to the games, Unity libraries, game artwork or firmware drivers.
These community ports are independent of the original developers and PortMaster.

## Triples

The demake is based on [Triples by Christopher Wellons](https://github.com/skeeto/triples),
commit `83e8286d15a1d3631d110dd47df88fd94d28c85a`, under the Unlicense.
RG35XX-H adaptations are by elarkitechto. Inter and Jua fonts retain their OFL
licenses in `ports/triples/licenses/`. See the demake’s
[credits](ports/triples/CREDITS.md) and [artwork licenses](ports/triples/ASSETS-LICENSE.md).

## Packaging

Package layouts follow [PortMaster](https://github.com/PortsMaster/PortMaster-New).
The ports have not yet been accepted into its official catalog.

## Katana ZERO

GMLoader-next by JohnnyonFlame and contributors, GPLv2, pinned revision
`522d964025e8ecb739c5aeb0f57afaf6f7927849`. Katana-specific loader patches and
packaging by elarkitechto, GPLv2; see [license](ports/katanazero/LICENSE).
Katana ZERO belongs to Askiisoft / Devolver Digital; Android SDK and services
are associated with Netflix. Proprietary game files and artwork are excluded.
The root Apache-2.0 framework license does not relicense this component.

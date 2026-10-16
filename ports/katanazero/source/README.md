# Source and packaging

Upstream: https://github.com/JohnnyonFlame/gmloader-next
Pinned revision: `522d964025e8ecb739c5aeb0f57afaf6f7927849`.
Apply `gmloader-katana.patch` using `build.sh`. Build on Linux ARM64 with the
upstream documented compiler, SDL2/zlib headers and Python clang bindings.
Pass `LLVM_FILE` and `LLVM_INC` as needed. Older clang Python bindings are expected
by the upstream generator; clang 19 needs adaptation of its enum API.

The patch registers java/lang/Object and boxed Double, fixes Object reference
arrays, links dsMapAddDouble, and implements the observed Katana extension calls.
The local compatibility path is gated by `KATANA_UNLOCKED_COMPAT=1`. The historical
name identifies the implementation behavior; installation accepts the verified
original XAPK components. No Java/Dex VM or real Netflix authentication is provided.

Build the presentation adapter on ARM64 Linux:
`source/build-controller-mode.sh PATH_TO_LOADER libkatana-controller-mode.so`.
It derives loader offsets from its symbol table and writes a matching SHA-256
sidecar. Do not strip the loader before this step. `CC` and `NM` may select a
cross toolchain. The engine address/slot is pinned to the verified 1.1.10 engine.
The launcher rejects mismatched loader/engine files before using the adapter.
The adapter checks the scalar `gamepadMode` as real or boolean and sets it to
controller mode without synthetic input; save files are not edited.

Use `package.py --controller-shim libkatana-controller-mode.so --loader PATH --libm PATH --cover COVER.png --screenshot SCREENSHOT.png --output PATH`.
Supply a vertical cover and a separate gameplay screenshot with appropriate
redistribution rights. They are not stored in the public source tree. The loader must be built
from the pinned source plus this patch. Supply the upstream ARM64 Android `libm.so`
from that same checkout. KNULLI supplies the native host libraries.
The package tool excludes APKs, extracted game files, saves and logs. Retain the
upstream source and all third-party license notices when distributing binaries;
GPLv2 source obligations also apply. Never publish proprietary runtime libraries.

## Acceptance before release

- Fresh official 1.1.10 XAPK setup from one launcher invocation.
- Native controls, audible music/effects, F+Start returning to Ports.
- Gameplay beyond the tutorial with Bloom off.
- Progress survives exit and relaunch; no seed save in the package.
- No shutdown SIGSEGV; inspect OpenAL/OGG warnings.
- Validate PortMaster metadata and supply separately licensed cover/screenshot.

The current project is a candidate, not an assertion that all checks pass.

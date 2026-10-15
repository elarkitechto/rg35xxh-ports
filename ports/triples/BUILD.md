# Build Triples

Development fork of [Triples](https://github.com/skeeto/triples), based on commit
`83e8286d15a1d3631d110dd47df88fd94d28c85a`. The SDL3 engine was adapted to
SDL2 for KNULLI. This demake is not ready for PortMaster.

## Desktop

Linux requires CMake 3.24+, a C++20 compiler and SDL2 2.0.14 or newer:

```sh
cmake -S . -B build -DTHREES_SYSTEM_SDL2=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
```

Windows requires MinGW-w64 and CMake. SDL2 is downloaded and linked statically:

```sh
cmake -S . -B build -G "MinGW Makefiles" -DTHREES_SYSTEM_SDL2=OFF
cmake --build build -j
```

## ARM64 handheld

With Zig 0.14.1 and Python, from this directory:

```sh
python tools/get_arm_sdk.py build-sdk
python tools/build_arm.py --zig /path/to/zig --sdk build-sdk --out build-arm
```

Debian SDL2 is used only for linking. The handheld executable uses the firmware’s
`libSDL2-2.0.so.0` and requires glibc symbols up to version 2.27. Do not replace
the handheld’s SDL2 library.

The package tool accepts `--arm`, `--destination` and optional `--windows`
arguments. Keep `Triples.sh` beside `threes-knulli/` when installing a build.
Do not overwrite `threes-knulli/saves/` during updates.

## Assets and licenses

The public build uses code-generated Triples tiles. Inter and Jua fonts are
included under the OFL licenses in `licenses/`. Locally prepared artwork can be
included with `--private-skin` in the ARM build and package tools; those builds
must not be redistributed without permission for the artwork.

Triples code is under the Unlicense. Original Threes! artwork has separate
ownership; see [ASSETS-LICENSE.md](ASSETS-LICENSE.md) and [CREDITS.md](CREDITS.md).
[README.upstream.md](README.upstream.md) preserves the upstream documentation.

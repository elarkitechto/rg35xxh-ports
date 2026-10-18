#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
: "${ALTO_LINK_LIBS:?Set ALTO_LINK_LIBS to the directory containing KNULLI ARM64 libSDL2.so, libmali.so and libz.so}"
ZIG=${ZIG_BIN:-zig}
SDL_INCLUDE=${SDL_INCLUDE_DIR:-$(pkg-config --variable=includedir sdl2)}
OUT=${ALTO_BUILD_DIR:-$ROOT/../../../.local/alto-build}
export ZIG_GLOBAL_CACHE_DIR=${ZIG_GLOBAL_CACHE_DIR:-$OUT/zig-global-cache}
export ZIG_LOCAL_CACHE_DIR=${ZIG_LOCAL_CACHE_DIR:-$OUT/zig-local-cache}
mkdir -p "$OUT"
cd "$ROOT"
"$ZIG" cc -ffile-prefix-map="$ROOT"=/build/ports/altosadventure -fdebug-prefix-map="$HOME"=/build/home -target aarch64-linux-gnu -O2 -g -fno-pie -no-pie -rdynamic -Wl,--allow-shlib-undefined \
 -I src -I include -I "$SDL_INCLUDE" \
 src/main_alto.c src/alto_policy.c src/alto_ui.c src/so_util.c src/jni_shim.c src/imports_unity.gen.c \
 src/jni_idx_stubs.gen.c src/recon_egl.c src/pthread_shim.c src/egl_shim.c src/opensles_shim.c src/util.c src/error.c \
 "$ALTO_LINK_LIBS/libSDL2.so" "$ALTO_LINK_LIBS/libmali.so" "$ALTO_LINK_LIBS/libz.so" -lm -ldl -lpthread -o "$OUT/alto-loader"
"$ZIG" cc -ffile-prefix-map="$ROOT"=/build/ports/altosadventure -fdebug-prefix-map="$HOME"=/build/home -target aarch64-linux-gnu -O2 -shared -fPIC -I src -I "$SDL_INCLUDE" \
 src/opensles_shim.c src/crossy_opensl_entry.c "$ALTO_LINK_LIBS/libSDL2.so" -ldl -lpthread -o "$OUT/libOpenSLES.so"

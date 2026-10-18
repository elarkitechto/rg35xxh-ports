#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
: "${CROSSY_LINK_LIBS:?Set CROSSY_LINK_LIBS to target SDL2, Mali and zlib link libraries}"
ZIG=${ZIG_BIN:-zig}
SDL_INCLUDE=${SDL_INCLUDE_DIR:-$(pkg-config --variable=includedir sdl2)}
OUT=${CROSSY_BUILD_DIR:-$ROOT/../../../.local/crossy-build}
mkdir -p "$OUT"
export ZIG_GLOBAL_CACHE_DIR=${ZIG_GLOBAL_CACHE_DIR:-$OUT/zig-global-cache}
export ZIG_LOCAL_CACHE_DIR=${ZIG_LOCAL_CACHE_DIR:-$OUT/zig-local-cache}
cd "$ROOT"
"$ZIG" cc -ffile-prefix-map="$ROOT"=/build/ports/crossyroad -fdebug-prefix-map="$HOME"=/build/home -target aarch64-linux-gnu -O2 -g -fno-pie -no-pie -rdynamic -Wl,--allow-shlib-undefined \
 -I runtime -I include -I "$SDL_INCLUDE" \
 runtime/main_crossy.c runtime/so_util.c runtime/jni_shim.c runtime/imports_unity.gen.c \
 runtime/jni_idx_stubs.gen.c runtime/recon_egl.c runtime/pthread_shim.c runtime/egl_shim.c runtime/util.c runtime/error.c \
 "$CROSSY_LINK_LIBS/libSDL2.so" "$CROSSY_LINK_LIBS/libmali.so" "$CROSSY_LINK_LIBS/libz.so" -lm -ldl -lpthread -o "$OUT/crossy-loader"
"$ZIG" cc -ffile-prefix-map="$ROOT"=/build/ports/crossyroad -fdebug-prefix-map="$HOME"=/build/home -target aarch64-linux-gnu -shared -fPIC -O2 -g jni_override.c -ldl -lpthread -o "$OUT/libcrossy-jni-native.so"
"$ZIG" cc -ffile-prefix-map="$ROOT"=/build/ports/crossyroad -fdebug-prefix-map="$HOME"=/build/home -target aarch64-linux-gnu -shared -fPIC -O2 -I runtime -I "$SDL_INCLUDE" \
 runtime/opensles_shim.c runtime/crossy_opensl_entry.c "$CROSSY_LINK_LIBS/libSDL2.so" -ldl -lpthread -o "$OUT/libOpenSLES.so"

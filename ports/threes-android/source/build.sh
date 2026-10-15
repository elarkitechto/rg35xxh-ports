#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
: "${THREES_LINK_LIBS:?Set THREES_LINK_LIBS to the target directory containing libSDL2.so, libmali.so and libz.so}"
ZIG=${ZIG_BIN:-zig}
: "${THREES_SDK_ROOT:?Set THREES_SDK_ROOT to the target sysroot usr directory}"
SDK=$THREES_SDK_ROOT
OUT=${THREES_BUILD_DIR:-$ROOT/../../../.local/threes-android-build}
mkdir -p "$OUT"
export ZIG_GLOBAL_CACHE_DIR=${ZIG_GLOBAL_CACHE_DIR:-$OUT/zig-global-cache}
export ZIG_LOCAL_CACHE_DIR=${ZIG_LOCAL_CACHE_DIR:-$OUT/zig-local-cache}
cd "$ROOT"
"$ZIG" cc -target aarch64-linux-gnu -O2 -g -fno-pie -no-pie -rdynamic -Wl,--allow-shlib-undefined \
  -I runtime -I runtime/include -I "$SDK/include" -I "$SDK/include/aarch64-linux-gnu" \
  runtime/main_threes.c runtime/threes_profile.c runtime/threes_display.c runtime/threes_input.c runtime/threes_ui.c runtime/threes_offline.c runtime/so_util.c runtime/jni_shim.c runtime/imports_unity.gen.c \
  runtime/jni_idx_stubs.gen.c runtime/recon_egl.c runtime/pthread_shim.c runtime/egl_shim.c \
  runtime/util.c runtime/error.c runtime/opensles_shim.c runtime/crossy_opensl_entry.c "$THREES_LINK_LIBS/libSDL2.so" \
  "$THREES_LINK_LIBS/libmali.so" "$THREES_LINK_LIBS/libz.so" -lm -ldl -lpthread \
  -o "$OUT/threes-loader"
"$ZIG" cc -target aarch64-linux-gnu -shared -fPIC -O2 -I runtime -I runtime/include \
  -I "$SDK/include" runtime/opensles_shim.c runtime/crossy_opensl_entry.c "$THREES_LINK_LIBS/libSDL2.so" \
  -ldl -lpthread -o "$OUT/libOpenSLES.so"
printf 'Built %s and libOpenSLES.so\n' "$OUT/threes-loader"

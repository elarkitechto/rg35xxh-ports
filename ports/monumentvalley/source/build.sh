#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
: "${MONUMENT_LINK_LIBS:?Set MONUMENT_LINK_LIBS to the target directory containing libSDL2.so, libmali.so and libz.so}"
ZIG=${ZIG_BIN:-zig}
: "${MONUMENT_SDK_ROOT:?Set MONUMENT_SDK_ROOT to the target sysroot usr directory}"
SDK=$MONUMENT_SDK_ROOT
OUT=${MONUMENT_BUILD_DIR:-$ROOT/../../../.local/monumentvalley-android-build}
mkdir -p "$OUT"
export ZIG_GLOBAL_CACHE_DIR=${ZIG_GLOBAL_CACHE_DIR:-$OUT/zig-global-cache}
export ZIG_LOCAL_CACHE_DIR=${ZIG_LOCAL_CACHE_DIR:-$OUT/zig-local-cache}
cd "$ROOT"
PROFILE_FLAGS=
"$ZIG" cc $PROFILE_FLAGS -ffile-prefix-map="$ROOT"=/build/ports/monumentvalley -fdebug-prefix-map="$HOME"=/build/home -target aarch64-linux-gnu -O2 -g -fno-pie -no-pie -rdynamic -Wl,--allow-shlib-undefined \
  -I runtime -I runtime/include -I "$SDK/include" -I "$SDK/include/aarch64-linux-gnu" \
  runtime/main_monument.c runtime/monument_profile.c runtime/monument_display.c runtime/monument_input.c runtime/monument_offline.c runtime/so_util.c runtime/jni_shim.c runtime/imports_unity.gen.c \
  runtime/jni_idx_stubs.gen.c runtime/recon_egl.c runtime/pthread_shim.c runtime/egl_shim.c \
  runtime/util.c runtime/error.c runtime/opensles_shim.c runtime/crossy_opensl_entry.c "$MONUMENT_LINK_LIBS/libSDL2.so" \
  "$MONUMENT_LINK_LIBS/libmali.so" "$MONUMENT_LINK_LIBS/libz.so" -lm -ldl -lpthread \
  -o "$OUT/monument-loader"
"$ZIG" cc $PROFILE_FLAGS -ffile-prefix-map="$ROOT"=/build/ports/monumentvalley -fdebug-prefix-map="$HOME"=/build/home -target aarch64-linux-gnu -shared -fPIC -O2 -I runtime -I runtime/include \
  -I "$SDK/include" runtime/opensles_shim.c runtime/crossy_opensl_entry.c "$MONUMENT_LINK_LIBS/libSDL2.so" \
  -ldl -lpthread -o "$OUT/libOpenSLES.so"
printf 'Built %s and libOpenSLES.so\n' "$OUT/monument-loader"

#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/../../.." && pwd)
out="$root/.local/components"
mkdir -p "$out"
zig_bin=${ZIG_BIN:-zig}
export ZIG_GLOBAL_CACHE_DIR=${ZIG_GLOBAL_CACHE_DIR:-/tmp/zig-global-cache}
export ZIG_LOCAL_CACHE_DIR=${ZIG_LOCAL_CACHE_DIR:-/tmp/zig-local-cache}
"$zig_bin" cc -target aarch64-linux-gnu -shared -fPIC -O2 -g \
  "$root/ports/crossyroad/source/jni_override.c" -ldl -lpthread \
  -o "$out/libcrossy-jni-native.so"
echo "Built $out/libcrossy-jni-native.so"

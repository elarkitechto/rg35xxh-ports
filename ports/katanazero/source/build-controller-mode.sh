#!/bin/sh
set -eu
loader=${1:?Usage: build-controller-mode.sh LOADER OUTPUT.so}
output=${2:?Usage: build-controller-mode.sh LOADER OUTPUT.so}
root=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
symbol_offset() {
 value=$(${NM:-nm} -n "$loader" | awk -v symbol="$1" '$3 == symbol { print "0x" $1; exit }')
 [ -n "$value" ] || { echo "Missing loader symbol: $1" >&2; exit 1; }
 printf '%s' "$value"
}
${CC:-cc} -shared -fPIC -O2 "$root/controller-mode.c" -o "$output" -ldl \
 -DKATANA_GLOBAL_OFFSET="$(symbol_offset g_pGlobal)" \
 -DKATANA_GET_OFFSET="$(symbol_offset Variable_GetValue_Direct)" \
 -DKATANA_SET_OFFSET="$(symbol_offset Variable_SetValue_Direct)"
sha256sum "$loader" | awk '{print $1}' > "$output.loader.sha256"

#!/bin/sh
# One command compiles the loader, game bridge and OpenSL runtime, then packages.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
CROSSY_MEDIA_DIR=${CROSSY_MEDIA_DIR:-$ROOT/.local/components}
python3 "$ROOT/tools/validate-port-media.py" "$CROSSY_MEDIA_DIR"
export CROSSY_BUILD_DIR="$ROOT/.local/crossy-build"
sh "$ROOT/ports/crossyroad/source/build_all.sh"
STAGE=$(mktemp -d)
trap 'rm -rf "$STAGE"' 0 HUP INT TERM
cp -R "$ROOT/ports/crossyroad/portmaster/." "$STAGE/"
cp "$ROOT/ports/crossyroad/README.md" "$STAGE/crossyroad/README.md"
mkdir -p "$STAGE/crossyroad/runtime"
cp "$CROSSY_BUILD_DIR/crossy-loader" "$CROSSY_BUILD_DIR/libcrossy-jni-native.so" "$CROSSY_BUILD_DIR/libOpenSLES.so" "$STAGE/crossyroad/runtime"
cp "$CROSSY_MEDIA_DIR/screenshot.png" "$STAGE/screenshot.png"
cp "$CROSSY_MEDIA_DIR/cover.png" "$STAGE/cover.png"
cp "$CROSSY_MEDIA_DIR/cover.png" "$STAGE/crossyroad/cover.png"
mkdir -p "$STAGE/crossyroad/licenses"
cp "$ROOT/LICENSE" "$STAGE/crossyroad/licenses/Apache-2.0.txt"
cp "$ROOT/NOTICE" "$STAGE/crossyroad/licenses/NOTICE.txt"
cp "$ROOT/ports/crossyroad/source/include/KHR/khrplatform.h" "$STAGE/crossyroad/licenses/Khronos-permission-notice.h"
printf '%s\n' '0.2.0-rc2' > "$STAGE/crossyroad/version.txt"
(cd "$STAGE/crossyroad/runtime" && shasum -a 256 crossy-loader libcrossy-jni-native.so libOpenSLES.so > runtime.sha256)
python3 -m json.tool "$STAGE/port.json" >/dev/null
chmod +x "$STAGE/Crossy Road.sh" "$STAGE/crossyroad/runtime/crossy-loader"
mkdir -p "$ROOT/.local/releases"
rm -f "$ROOT/.local/releases/crossyroad.zip"
(cd "$STAGE" && zip -qr "$ROOT/.local/releases/crossyroad.zip" . -x '*/._*' '._*')
cp "$ROOT/.local/releases/crossyroad.zip" "$ROOT/.local/releases/crossyroad-0.2.0-rc2.zip"
(cd "$ROOT/.local/releases" && shasum -a 256 crossyroad-0.2.0-rc2.zip > crossyroad-0.2.0-rc2.zip.sha256)
echo "Built $ROOT/.local/releases/crossyroad-0.2.0-rc2.zip"

python3 "$ROOT/tools/prepare-portmaster.py" crossyroad --package "$ROOT/.local/releases/crossyroad.zip" --check-only

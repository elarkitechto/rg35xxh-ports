#!/bin/sh
# One host entry point: compile everything and produce the handheld package.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
: "${ALTO_MEDIA_DIR:?Set ALTO_MEDIA_DIR to the private screenshot.png and cover.png directory}"
python3 "$ROOT/tools/validate-port-media.py" "$ALTO_MEDIA_DIR"
export ALTO_BUILD_DIR="$ROOT/.local/alto-build"
sh "$ROOT/ports/altosadventure/source/build.sh"
STAGE=$(mktemp -d)
trap 'rm -rf "$STAGE"' 0 HUP INT TERM
cp -R "$ROOT/ports/altosadventure/portmaster/." "$STAGE/"
cp "$ROOT/ports/altosadventure/README.md" "$STAGE/altosadventure/README.md"
mkdir -p "$STAGE/altosadventure/runtime"
cp "$ALTO_BUILD_DIR/alto-loader" "$ALTO_BUILD_DIR/libOpenSLES.so" "$STAGE/altosadventure/runtime"
cp "$ALTO_MEDIA_DIR/screenshot.png" "$STAGE/screenshot.png"
cp "$ALTO_MEDIA_DIR/cover.png" "$STAGE/cover.png"
cp "$ALTO_MEDIA_DIR/cover.png" "$STAGE/altosadventure/cover.png"
mkdir -p "$STAGE/altosadventure/licenses"
cp "$ROOT/ports/altosadventure/LICENSE" "$STAGE/altosadventure/licenses/Apache-2.0.txt"
cp "$ROOT/ports/altosadventure/NOTICE" "$STAGE/altosadventure/licenses/NOTICE.txt"
cp "$ROOT/ports/altosadventure/source/include/KHR/khrplatform.h" "$STAGE/altosadventure/licenses/Khronos-permission-notice.h"
printf '%s\n' '0.2.0-rc2' > "$STAGE/altosadventure/version.txt"
(cd "$STAGE/altosadventure/runtime" && shasum -a 256 alto-loader libOpenSLES.so > runtime.sha256)
python3 -m json.tool "$STAGE/port.json" >/dev/null
chmod +x "$STAGE/Alto's Adventure.sh" "$STAGE/altosadventure/runtime/alto-loader"
mkdir -p "$ROOT/.local/releases"
# Replace old archive, so removed files cannot survive an update.
rm -f "$ROOT/.local/releases/altosadventure.zip"
(cd "$STAGE" && zip -qr "$ROOT/.local/releases/altosadventure.zip" . -x '*/._*' '._*')
python3 "$ROOT/tools/sanitize-release.py" "$ROOT/.local/releases/altosadventure.zip"
cp "$ROOT/.local/releases/altosadventure.zip" "$ROOT/.local/releases/altosadventure-0.2.0-rc2.zip"
(cd "$ROOT/.local/releases" && shasum -a 256 altosadventure-0.2.0-rc2.zip > altosadventure-0.2.0-rc2.zip.sha256)
echo "Built: $ROOT/.local/releases/altosadventure-0.2.0-rc2.zip"

python3 "$ROOT/tools/prepare-portmaster.py" altosadventure --package "$ROOT/.local/releases/altosadventure.zip" --check-only

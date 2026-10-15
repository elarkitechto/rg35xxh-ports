#!/bin/sh
# Compile and package the complete compatibility runtime; game data is user-supplied.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
: "${THREES_MEDIA_DIR:?Set THREES_MEDIA_DIR to cover.png and screenshot.png}"
python3 "$ROOT/tools/validate-port-media.py" "$THREES_MEDIA_DIR"
export THREES_BUILD_DIR=${THREES_BUILD_DIR:-$ROOT/.local/threes-android-build}
sh "$ROOT/ports/threes/source/build.sh"
python3 "$ROOT/ports/threes/tools/package.py" --runtime "$THREES_BUILD_DIR" --media "$THREES_MEDIA_DIR" --output "$ROOT/.local/releases/threes-0.1.0-rc1.zip"
cp "$ROOT/.local/releases/threes-0.1.0-rc1.zip" "$ROOT/.local/releases/threes.zip"
(cd "$ROOT/.local/releases" && shasum -a 256 threes-0.1.0-rc1.zip > threes-0.1.0-rc1.zip.sha256)

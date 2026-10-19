#!/bin/sh
# Compile and package the complete compatibility runtime; game data is user-supplied.
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
: "${MONUMENT_MEDIA_DIR:?Set MONUMENT_MEDIA_DIR to cover.png and screenshot.png}"
python3 "$ROOT/tools/validate-port-media.py" "$MONUMENT_MEDIA_DIR"
export MONUMENT_BUILD_DIR=${MONUMENT_BUILD_DIR:-$ROOT/.local/monumentvalley-android-build}
sh "$ROOT/ports/monumentvalley/source/build.sh"
python3 "$ROOT/ports/monumentvalley/tools/package.py" --runtime "$MONUMENT_BUILD_DIR" --media "$MONUMENT_MEDIA_DIR" --output "$ROOT/.local/releases/monumentvalley-0.1.1.zip"
python3 "$ROOT/tools/sanitize-release.py" "$ROOT/.local/releases/monumentvalley-0.1.1.zip"
cp "$ROOT/.local/releases/monumentvalley-0.1.1.zip" "$ROOT/.local/releases/monumentvalley.zip"
(cd "$ROOT/.local/releases" && shasum -a 256 monumentvalley-0.1.1.zip > monumentvalley-0.1.1.zip.sha256 && shasum -a 256 monumentvalley.zip > monumentvalley.zip.sha256)

python3 "$ROOT/tools/prepare-portmaster.py" monumentvalley --package "$ROOT/.local/releases/monumentvalley.zip" --check-only

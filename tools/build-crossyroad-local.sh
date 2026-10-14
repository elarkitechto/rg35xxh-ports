#!/bin/sh
# Assemble a private, tested package from components supplied outside Git.
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
components=${LOCAL_COMPONENTS_DIR:-$root/.local/components}
release="$root/.local/releases"
stage=$(mktemp -d)
trap 'rm -rf "$stage"' EXIT HUP INT TERM
mkdir -p "$stage/crossyroad" "$release"
cp "$root/ports/crossyroad/portmaster/Crossy Road.sh" "$root/ports/crossyroad/portmaster/port.json" "$root/ports/crossyroad/portmaster/gameinfo.xml" "$stage/"
cp "$root/ports/crossyroad/portmaster/crossyroad/install.sh" "$stage/crossyroad/"
cp "$root/LICENSE" "$stage/crossyroad/LICENSE.nextos.txt"
cp "$root/NOTICE" "$stage/crossyroad/NOTICE.nextos.txt"
check_copy() {
  name=$1 expected=$2 destination=$3
  [ -f "$components/$name" ] || { echo "Missing $components/$name" >&2; exit 1; }
  actual=$(shasum -a 256 "$components/$name" | cut -d ' ' -f 1)
  [ "$actual" = "$expected" ] || { echo "Unexpected SHA-256 for $name: $actual" >&2; exit 1; }
  cp "$components/$name" "$destination"
}
check_copy crossy-loader-sdl-single 0e611578009776ec0d5f2d309fe333dd3ccc1b81d45bbb179c41e74e6cbcb7b7 "$stage/crossyroad/"
check_copy libcrossy-jni-native.so 6763ad312ebb7cc09cd2a705d6aedf6a98d2e6003728efbbb74bafd892261348 "$stage/crossyroad/"
check_copy libOpenSLES.so 6f016fc1ef15d9d6c9594d77b840f15c071e9d66b8a38d4a0fcbcf97d3a0b5ab "$stage/crossyroad/"
check_copy libMali.so 08c4c5f65f31a753065cf7332b5596f045a552697cfd9849aef15ea494192b2b "$stage/crossyroad/"
check_copy cover.png 227c1360c3893d164fc979f50aa0996368872c782cad3534475b1a5c01f61ed7 "$stage/crossyroad/"
check_copy screenshot.png cb71fc14ce9671096ba64d8058e11fedd43315d7b3d25c8f2a9f9162510a5b19 "$stage/"
cp "$stage/screenshot.png" "$stage/crossyroad/screenshot.png"
cd "$stage"
zip -qr "$release/crossyroad.zip" 'Crossy Road.sh' port.json gameinfo.xml screenshot.png crossyroad
echo "Built private package: $release/crossyroad.zip"

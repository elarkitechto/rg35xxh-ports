#!/bin/sh
# Installs the exact tested Crossy Road 4.5.0 APK. Existing saves are untouched.
set -eu

PORTDIR=$1
SYSTEM_ROOT=$2
APK="$PORTDIR/crossy-road-4-5-0.apk"
LAB="$SYSTEM_ROOT/crossyroad-lab"
DATA="$SYSTEM_ROOT/cr"
EXPECTED=1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb

fail() { echo "Crossy Road: $*" >&2; exit 1; }
mkdir -p "$SYSTEM_ROOT" "$LAB" "$DATA"

# Unity Analytics stores offline events separately from game progress. On this
# device they grew to 212 MB and caused SD writes during play. Opt out before
# Unity starts and remove only its old event spool, never userdata saves/prefs.
configure_analytics() {
  ANALYTICS_DIR="$DATA/userdata/Unity/5503606a-f39e-4202-bd2b-3e96f3ab4629/Analytics"
  mkdir -p "$ANALYTICS_DIR"
  printf '%s\n' '{"analytics":{"enabled":false},"connect":{"enabled":false,"player_opted_out":true,"limit_user_tracking":true},"performance":{"enabled":false}}' > "$ANALYTICS_DIR/config"
  rm -rf "$ANALYTICS_DIR/ArchivedEvents"
  mkdir -p "$ANALYTICS_DIR/ArchivedEvents"
}

if [ "$(cat "$DATA/.crossyroad-apk.sha256" 2>/dev/null || true)" = "$EXPECTED" ] &&
   [ -s "$DATA/assets/bin/Data/data.unity3d" ] &&
   [ -s "$LAB/crossy-loader-sdl-single" ] &&
   [ -s "$LAB/libcrossy-jni-native.so" ] &&
   [ -s "$LAB/libMali.so" ] &&
   [ -s "$LAB/libOpenSLES.so" ] &&
   [ -s "$LAB/libmain.so" ] &&
   [ -s "$LAB/libil2cpp.so" ] &&
   [ -s "$LAB/libunity.so" ]; then
  configure_analytics
  echo 'Crossy Road: installation already complete.'
  exit 0
fi

[ -f "$APK" ] || fail "copy crossy-road-4-5-0.apk into $PORTDIR and launch again"
command -v sha256sum >/dev/null 2>&1 || fail 'sha256sum is required'
command -v unzip >/dev/null 2>&1 || fail 'unzip is required'
ACTUAL=$(sha256sum "$APK" | cut -d ' ' -f 1)
[ "$ACTUAL" = "$EXPECTED" ] || fail "wrong APK (SHA-256: $ACTUAL). Expected Crossy Road 4.5.0."
check_port_file() {
  [ -s "$PORTDIR/$1" ] || fail "port file missing: $1"
  HASH=$(sha256sum "$PORTDIR/$1" | cut -d ' ' -f 1)
  [ "$HASH" = "$2" ] || fail "port file has changed: $1"
}
check_port_file crossy-loader-sdl-single 0e611578009776ec0d5f2d309fe333dd3ccc1b81d45bbb179c41e74e6cbcb7b7
check_port_file libcrossy-jni-native.so 52406e5c69d7614070ab6a71fd311405cae98a470f1da7a78a341e15b69875ba
check_port_file libOpenSLES.so 6f016fc1ef15d9d6c9594d77b840f15c071e9d66b8a38d4a0fcbcf97d3a0b5ab
check_port_file libMali.so 08c4c5f65f31a753065cf7332b5596f045a552697cfd9849aef15ea494192b2b

STAGE=$(mktemp -d "$SYSTEM_ROOT/.crossyroad-install.XXXXXX") || fail 'cannot create staging directory'
trap 'rm -rf "$STAGE"' 0 HUP INT TERM
unzip -oq "$APK" 'assets/*' 'lib/arm64-v8a/libmain.so' 'lib/arm64-v8a/libil2cpp.so' 'lib/arm64-v8a/libunity.so' -d "$STAGE" || fail 'APK extraction failed'
[ -s "$STAGE/assets/bin/Data/data.unity3d" ] || fail 'Unity assets are missing from APK'
[ -s "$STAGE/assets/bin/Data/Managed/Metadata/global-metadata.dat" ] || fail 'IL2CPP metadata is missing from APK'

# Keep current saves and Unity user data. On an existing installation the
# assets from this same APK need no replacement.
if [ ! -s "$DATA/assets/bin/Data/data.unity3d" ]; then
  if [ -d "$DATA/assets" ]; then
    fail "$DATA/assets exists but is incomplete; inspect it before installation"
  fi
  mv "$STAGE/assets" "$DATA/assets"
else
  cmp -s "$STAGE/assets/bin/Data/data.unity3d" "$DATA/assets/bin/Data/data.unity3d" || fail 'existing Unity assets differ from this APK; installation stopped without replacing them'
  cmp -s "$STAGE/assets/bin/Data/Managed/Metadata/global-metadata.dat" "$DATA/assets/bin/Data/Managed/Metadata/global-metadata.dat" || fail 'existing IL2CPP metadata differs from this APK; installation stopped without replacing it'
fi

for name in libmain.so libil2cpp.so libunity.so; do
  cp "$STAGE/lib/arm64-v8a/$name" "$LAB/$name"
done
for name in crossy-loader-sdl-single libcrossy-jni-native.so libOpenSLES.so libMali.so; do
  cp "$PORTDIR/$name" "$LAB/$name"
done
chmod +x "$LAB/crossy-loader-sdl-single"
mkdir -p "$DATA/userdata"
configure_analytics
printf '%s\n' "$EXPECTED" > "$DATA/.crossyroad-apk.sha256"
echo 'Crossy Road: APK verified; assets, libraries and loader installed.'

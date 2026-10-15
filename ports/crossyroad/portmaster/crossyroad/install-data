#!/bin/sh
# Called automatically by the only top-level game launcher; never run manually.
set -eu
PORTDIR=$1
LAB=${CROSSY_RUNTIME_DIR:-$PORTDIR/runtime}
DATA=${CROSSY_DATA_DIR:-$PORTDIR/gamedata}
APK="$PORTDIR/crossy-road-4-5-0.apk"
EXPECTED=1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb
fail() { echo "Crossy Road: $*" >&2; exit 1; }
mkdir -p "$LAB" "$DATA"
cd "$PORTDIR"
sha256sum -c runtime.sha256 || fail 'port runtime is missing or damaged'
# An installed game still receives new loader versions without replacing saves.
if [ "$(cat "$DATA/.crossy-apk.sha256" 2>/dev/null || true)" != "$EXPECTED" ] ||
   [ ! -s "$DATA/assets/bin/Data/data.unity3d" ] ||
   [ ! -s "$DATA/assets/bin/Data/Managed/Metadata/global-metadata.dat" ] ||
   ! (cd "$LAB" && sha256sum -c "$PORTDIR/crossy-libraries.sha256" >/dev/null 2>&1); then
 [ -f "$APK" ] || fail "copy crossy-road-4-5-0.apk into $PORTDIR and launch again"
 [ "$(sha256sum "$APK" | cut -d ' ' -f 1)" = "$EXPECTED" ] || fail 'wrong APK; Crossy Road 4.5.0 with the documented SHA-256 is required'
 STAGE=$(mktemp -d "$PORTDIR/.crossy-install.XXXXXX")
 trap 'rm -rf "$STAGE"' 0 HUP INT TERM
 unzip -oq "$APK" 'assets/*' 'lib/arm64-v8a/libil2cpp.so' 'lib/arm64-v8a/libunity.so' -d "$STAGE"
 [ -s "$STAGE/assets/bin/Data/data.unity3d" ] || fail 'Unity assets missing'
 [ -s "$STAGE/assets/bin/Data/Managed/Metadata/global-metadata.dat" ] || fail 'IL2CPP metadata missing'
 (cd "$STAGE/lib/arm64-v8a" && sha256sum -c "$PORTDIR/crossy-libraries.sha256") || fail 'unexpected Unity libraries'
 if [ -d "$DATA/assets" ]; then
  cmp -s "$DATA/assets/bin/Data/data.unity3d" "$STAGE/assets/bin/Data/data.unity3d" || fail 'existing assets differ; refusing to replace them'
  cmp -s "$DATA/assets/bin/Data/Managed/Metadata/global-metadata.dat" "$STAGE/assets/bin/Data/Managed/Metadata/global-metadata.dat" || fail 'existing metadata differs'
 else
  mv "$STAGE/assets" "$DATA/assets"
 fi
 for name in libil2cpp.so libunity.so; do
  cp "$STAGE/lib/arm64-v8a/$name" "$LAB/$name.next"
  mv "$LAB/$name.next" "$LAB/$name"
 done
 printf '%s\n' "$EXPECTED" > "$DATA/.crossy-apk.sha256"
fi
for name in crossy-loader libcrossy-jni-native.so libOpenSLES.so; do
 if ! cmp -s "$PORTDIR/$name" "$LAB/$name"; then
  cp "$PORTDIR/$name" "$LAB/$name.next"
  mv "$LAB/$name.next" "$LAB/$name"
 fi
done
chmod +x "$LAB/crossy-loader"
ANALYTICS="$DATA/userdata/Unity/5503606a-f39e-4202-bd2b-3e96f3ab4629/Analytics"
mkdir -p "$ANALYTICS"
printf '%s\n' '{"analytics":{"enabled":false},"connect":{"enabled":false,"player_opted_out":true,"limit_user_tracking":true},"performance":{"enabled":false}}' > "$ANALYTICS/config"
echo "Crossy Road: ready."

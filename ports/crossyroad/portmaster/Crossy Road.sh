#!/bin/bash
# PORTMASTER: crossyroad.zip, Crossy Road.sh
XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}
if [ -n "${CROSSY_PORTMASTER_DIR:-}" ]; then
  controlfolder="${CROSSY_PORTMASTER_DIR}"
elif [ -d /opt/system/Tools/PortMaster ]; then
  controlfolder=/opt/system/Tools/PortMaster
elif [ -d /opt/tools/PortMaster ]; then
  controlfolder=/opt/tools/PortMaster
elif [ -d "$XDG_DATA_HOME/PortMaster" ]; then
  controlfolder="$XDG_DATA_HOME/PortMaster"
else
  controlfolder=/roms/ports/PortMaster
fi
export controlfolder
[ -f "$controlfolder/control.txt" ] || { echo "Crossy Road requires PortMaster." >&2; exit 1; }
source "$controlfolder/control.txt"
[ ! -f "$controlfolder/mod_${CFW_NAME}.txt" ] || source "$controlfolder/mod_${CFW_NAME}.txt"
get_controls

PORTDIR="$(CDPATH= cd -- "$(dirname -- "$0")/crossyroad" && pwd)" || exit 1
RUNTIME="$PORTDIR/runtime"
child_pid=
cleanup() {
  if [ -n "$child_pid" ] && kill -0 "$child_pid" 2>/dev/null; then kill -TERM "$child_pid"; wait "$child_pid"; fi
  pm_finish
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM
case "${DEVICE_ARCH}" in aarch64|arm64) ;; *) pm_message 'This port requires ARM64.'; exit 1;; esac
# Preserve saves from earlier KNULLI installations; fresh installs stay in this port.
if [ -z "${CROSSY_DATA_DIR:-}" ]; then
  if [ -d /userdata/system/cr/prefs ]; then export CROSSY_DATA_DIR=/userdata/system/cr;
  else export CROSSY_DATA_DIR="$PORTDIR/gamedata"; fi
fi
export CROSSY_RUNTIME_DIR="$RUNTIME" CROSSY_APK_PATH="$PORTDIR/crossy-road-4-5-0.apk"
if ! sh "$PORTDIR/install-data" "$PORTDIR" > "$PORTDIR/install.log" 2>&1; then
  cat "$PORTDIR/install.log" >&2
  pm_message 'Game setup failed. Check install.log and the supported game-file version.'
  exit 1
fi
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
case "$CFW_NAME" in KNULLI|knulli) export SDL_VIDEODRIVER=mali ;; esac
export LD_LIBRARY_PATH="$RUNTIME:/usr/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export HK_PTHREAD_SHIM=1 HK_SDLPAD=1 HK_FRAMES=0
export CROSSY_NATIVE_INPUT=1 CROSSY_FIX_INPUT_EVENT=1 CROSSY_FORCE_KEYBOARD=1
export CROSSY_SKIP_WAIT=1 CROSSY_SKIP_ANALYTICS=1 CROSSY_SKIP_BILLING=1
export CROSSY_SKIP_ANDROID_TIME=1 CROSSY_FIX_READLINK=1 CROSSY_DIAG_SKIP_AGE_GATE=1
export CROSSY_SYNC_SAVE=1 CROSSY_FORCE_OPENSL=1 CROSSY_AUDIO_SDK=25 CROSSY_AUDIO_PROPERTIES=1
export CROSSY_PULSE_DIRECT=1 CROSSY_HAPTICS=1 CROSSY_HAPTIC_ENVIRONMENT=1
for name in ${!CROSSY_TEST_@} ${!CROSSY_TRACE_@} ${!CROSSY_AUTO@}; do unset "$name"; done
unset CROSSY_CAPTURE_DIR CROSSY_SECONDS CROSSY_PREFS_DIR CROSSY_FORCE_CONTROLLER_UI CROSSY_PAD_BRIDGE CROSSY_DIRECT_GAME_INPUT
ulimit -c 0
cd "$RUNTIME" || exit 1
pm_platform_helper "$RUNTIME/crossy-loader"
LD_PRELOAD="$RUNTIME/libcrossy-jni-native.so" ./crossy-loader > "$PORTDIR/log.txt" 2>&1 &
child_pid=$!
wait "$child_pid"
status=$?
child_pid=
[ "$status" -eq 0 ] || pm_message "Game stopped (exit $status). Check crossyroad/log.txt."
exit "$status"

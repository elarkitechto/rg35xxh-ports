#!/bin/bash
# PORTMASTER: threes.zip, Threes!.sh
XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}
if [ -n "${THREES_PORTMASTER_DIR:-}" ]; then
  controlfolder="${THREES_PORTMASTER_DIR}"
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
[ -f "$controlfolder/control.txt" ] || { echo "Threes! requires PortMaster." >&2; exit 1; }
source "$controlfolder/control.txt"
[ ! -f "$controlfolder/mod_${CFW_NAME}.txt" ] || source "$controlfolder/mod_${CFW_NAME}.txt"
get_controls

PORTDIR="$(CDPATH= cd -- "$(dirname -- "$0")/threes" && pwd)" || exit 1
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
SAVE="${THREES_SAVE_DIR:-/userdata/system/threes-android/saves}"
mkdir -p "$SAVE"
export THREES_DATA_DIR="$PORTDIR/gamedata" THREES_SAVE_DIR="$SAVE" THREES_PREFS_DIR="$SAVE/prefs"
export THREES_APK_PATH="$PORTDIR/threes-3.1.6144.apk" THREES_RUNTIME_DIR="$RUNTIME"
if ! sh "$PORTDIR/install-data" "$PORTDIR" > "$PORTDIR/install.log" 2>&1; then
  cat "$PORTDIR/install.log" >&2
  pm_message 'Game setup failed. Check install.log and the supported game-file version.'
  exit 1
fi
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
case "$CFW_NAME" in KNULLI|knulli) export SDL_VIDEODRIVER=mali THREES_PULSE_DIRECT=1 ;; esac
export LD_LIBRARY_PATH="$RUNTIME:/usr/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export THREES_SDLPAD=1 THREES_GLES3=1 HK_PTHREAD_SHIM=1 HK_NOSPOOF=1
unset THREES_TATE THREES_FRAMES THREES_SECONDS THREES_CAPTURE THREES_TRACE_INPUT THREES_TEST_INPUT THREES_TEST_ROTATE THREES_TEST_CONTROLS
ulimit -c 0
cd "$RUNTIME" || exit 1
pm_platform_helper "$RUNTIME/threes-loader"
./threes-loader > "$PORTDIR/log.txt" 2>&1 &
child_pid=$!
wait "$child_pid"
status=$?
child_pid=
[ "$status" -eq 0 ] || pm_message "Game stopped (exit $status). Check threes/log.txt."
exit "$status"

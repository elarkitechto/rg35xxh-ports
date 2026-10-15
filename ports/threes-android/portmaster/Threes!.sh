#!/bin/bash
# PORTMASTER: threesandroid.zip, Threes!.sh
PORTDIR="$(CDPATH= cd -- "$(dirname -- "$0")/threesandroid" && pwd)" || exit 1
export XDG_DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
if [ -n "${THREES_PORTMASTER_DIR:-}" ]; then
  controlfolder="$THREES_PORTMASTER_DIR"
elif [ -f /opt/system/Tools/PortMaster/control.txt ]; then
  controlfolder=/opt/system/Tools/PortMaster
elif [ -f /opt/tools/PortMaster/control.txt ]; then
  controlfolder=/opt/tools/PortMaster
elif [ -f "$XDG_DATA_HOME/PortMaster/control.txt" ]; then
  controlfolder="$XDG_DATA_HOME/PortMaster"
elif [ -f /PortMaster/control.txt ]; then
  controlfolder=/PortMaster
else
  controlfolder="$(dirname "$PORTDIR")/PortMaster"
fi
export controlfolder
if [ ! -f "$controlfolder/control.txt" ]; then
  echo "Threes! requires PortMaster. Install or update PortMaster and relaunch." >&2
  exit 1
fi
source "$controlfolder/control.txt"
mod_script="$controlfolder/mod_${CFW_NAME:-}.txt"
[ -f "$mod_script" ] || { [ "${CFW_NAME:-}" != knulli ] || mod_script="$controlfolder/mod_KNULLI.txt"; }
[ ! -f "$mod_script" ] || source "$mod_script"
get_controls
message() { echo "Threes!: $*"; if declare -F pm_message >/dev/null; then pm_message "Threes!: $*"; fi; }
error_message() { message "$*"; sleep "${THREES_ERROR_DELAY:-6}"; }
case "${DEVICE_ARCH:-$(uname -m)}" in aarch64|arm64) ;; *) error_message 'This port requires ARM64.'; exit 1;; esac
DATA="$PORTDIR/gamedata"
RUNTIME="$PORTDIR/runtime"
SAVE="${THREES_SAVE_DIR:-/userdata/system/threes-android/saves}"
mkdir -p "$DATA" "$RUNTIME" "$SAVE" || exit 1
if ! sh "$PORTDIR/install-data" "$PORTDIR" > "$PORTDIR/install.log" 2>&1; then
  cat "$PORTDIR/install.log" >&2
  error_message 'Setup failed. See threesandroid/install.log; provide your Threes! 3.1 ARM64 XAPK and launch again.'
  exit 1
fi
[ -z "${sdl_controllerconfig:-}" ] || export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
if [ "${CFW_NAME:-}" = KNULLI ] || [ "${CFW_NAME:-}" = knulli ]; then export SDL_VIDEODRIVER=mali THREES_PULSE_DIRECT=1; fi
export LD_LIBRARY_PATH="$RUNTIME:/usr/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export THREES_DATA_DIR="$DATA"
export THREES_SAVE_DIR="$SAVE"
export THREES_PREFS_DIR="$SAVE/prefs"
export THREES_APK_PATH="$DATA/vo.threes.free.apk"
export THREES_RUNTIME_DIR="$RUNTIME"
export THREES_SDLPAD=1
export THREES_TATE=1
export HK_PTHREAD_SHIM=1 HK_NOSPOOF=1
export THREES_GLES3=1
unset THREES_FRAMES THREES_SECONDS THREES_CAPTURE THREES_TRACE_INPUT THREES_TEST_INPUT THREES_TEST_ROTATE THREES_TEST_CONTROLS
ulimit -c 0
if declare -F pm_platform_helper >/dev/null; then pm_platform_helper "$RUNTIME/threes-loader"; fi
printf '%s\n' "Threes! 3.1" "CFW=${CFW_NAME:-unknown} ARCH=${DEVICE_ARCH:-unknown}" "DATA=$DATA" "SAVE=$SAVE" "RUNTIME=$RUNTIME" "SDL_BACKEND=${SDL_VIDEODRIVER:-automatic}" > "$PORTDIR/log.txt"
cd "$RUNTIME" || exit 1
./threes-loader >> "$PORTDIR/log.txt" 2>&1 &
child_pid=$!
cleanup() {
  if [ -n "${child_pid:-}" ] && kill -0 "$child_pid" 2>/dev/null; then kill -TERM "$child_pid" 2>/dev/null; wait "$child_pid" 2>/dev/null; fi
  if declare -F pm_finish >/dev/null; then pm_finish; fi
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM
wait "$child_pid"
status=$?
child_pid=
if [ "$status" -ne 0 ]; then error_message "Game stopped (exit $status). Check threesandroid/log.txt."; fi
exit "$status"

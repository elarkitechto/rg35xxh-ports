#!/bin/bash
# PORTMASTER: altosadventure.zip, Alto's Adventure.sh
# Testing candidate 0.2.0-rc1. Only RG35XX-H/KNULLI is currently verified.
PORTDIR="$(CDPATH= cd -- "$(dirname -- "$0")/altosadventure" && pwd)" || exit 1
export XDG_DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
if [ -n "${ALTO_PORTMASTER_DIR:-}" ]; then
 controlfolder="$ALTO_PORTMASTER_DIR"
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
 echo "Alto's Adventure requires PortMaster. Please install or update it." >&2
 exit 1
fi
# PortMaster's scripts do not assume errexit/nounset.
source "$controlfolder/control.txt"
case "${CFW_NAME:-}" in KNULLI|knulli) alto_knulli=1;; *) alto_knulli=0;; esac
mod_script="$controlfolder/mod_${CFW_NAME:-}.txt"
if [ ! -f "$mod_script" ] && [ "$alto_knulli" = 1 ]; then mod_script="$controlfolder/mod_KNULLI.txt"; fi
[ ! -f "$mod_script" ] || source "$mod_script"
get_controls
child_pid=
cleanup() {
 if [ -n "$child_pid" ] && kill -0 "$child_pid" 2>/dev/null; then
  kill -TERM "$child_pid" 2>/dev/null
  wait "$child_pid" 2>/dev/null
 fi
 if declare -F pm_finish >/dev/null; then pm_finish; fi
}
trap cleanup EXIT
trap 'exit 130' INT
trap 'exit 143' TERM
message() {
 echo "Alto's Adventure: $*"
 if declare -F pm_message >/dev/null; then pm_message "Alto's Adventure: $*"; fi
}
error_message() { message "$@"; sleep "${ALTO_ERROR_DELAY:-6}"; }
case "${DEVICE_ARCH:-$(uname -m)}" in aarch64|arm64) ;; *) error_message 'This candidate requires ARM64.'; exit 1;; esac
export ALTO_RUNTIME_DIR="$PORTDIR/runtime"
# Keep the existing private KNULLI installation's saves in place. Fresh installs
# and other systems use self-contained port data; no automatic save copying.
if [ -z "${ALTO_DATA_DIR:-}" ]; then
 if [ "$alto_knulli" = 1 ] && [ -d /userdata/system/alto/prefs ]; then
  export ALTO_DATA_DIR=/userdata/system/alto
 else
  export ALTO_DATA_DIR="$PORTDIR/gamedata"
 fi
fi
export ALTO_APK_PATH="$PORTDIR/altos-adventure-1-8-8.apk"
mkdir -p "$ALTO_RUNTIME_DIR" || exit 1
export LD_LIBRARY_PATH="$ALTO_RUNTIME_DIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
if ! sh "$PORTDIR/install-data" "$PORTDIR" > "$PORTDIR/install.log" 2>&1; then
 cat "$PORTDIR/install.log" >&2
 error_message "Setup failed. Check altosadventure/install.log; supply the exact Alto 1.8.8 APK and launch again."
 exit 1
fi
# Use PortMaster's controller database; preserve a supplied mapping if its
# get_controls implementation uses SDL_GAMECONTROLLERCONFIG_FILE instead.
[ -z "${sdl_controllerconfig:-}" ] || export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
# KNULLI's tested SDL backend. Other backends remain experimental, not forced.
if [ "$alto_knulli" = 1 ]; then export SDL_VIDEODRIVER=mali; fi
if command -v ldd >/dev/null && ldd "$ALTO_RUNTIME_DIR/alto-loader" 2>&1 | grep -q 'not found'; then
 ldd "$ALTO_RUNTIME_DIR/alto-loader" > "$PORTDIR/runtime-dependencies.log" 2>&1
 error_message 'Missing runtime libraries. This candidate is verified only on RG35XX-H/KNULLI; see runtime-dependencies.log.'
 exit 1
fi
cd "$ALTO_RUNTIME_DIR" || exit 1
export HK_PTHREAD_SHIM=1 HK_NOSPOOF=1 HK_SDLPAD=1
export ALTO_FRAMES=0 ALTO_SECONDS=0 ALTO_OPENSL=1 CROSSY_PULSE_DIRECT=1
unset ALTO_OFFLINE_UI_SMOKE ALTO_DIALOG_SMOKE ALTO_AUTOPRESS ALTO_SETTINGS_SMOKE ALTO_UI_SMOKE ALTO_HAPTIC_TEST ALTO_CAPTURE ALTO_LEGACY_PACING ALTO_PREFS_DIR
ulimit -c 0
if declare -F pm_platform_helper >/dev/null; then pm_platform_helper "$ALTO_RUNTIME_DIR/alto-loader"; fi
printf '%s\n' "Alto's Adventure 0.2.0-rc1" "CFW=${CFW_NAME:-unknown} ARCH=${DEVICE_ARCH:-unknown}" "DATA=$ALTO_DATA_DIR" "RUNTIME=$ALTO_RUNTIME_DIR" "SDL_BACKEND=${SDL_VIDEODRIVER:-automatic}" > "$PORTDIR/log.txt"
./alto-loader >> "$PORTDIR/log.txt" 2>&1 &
child_pid=$!
wait "$child_pid"
status=$?
child_pid=
if [ "$status" -ne 0 ]; then error_message "Game stopped (exit $status). Please attach altosadventure/log.txt to your test report."; fi
exit "$status"

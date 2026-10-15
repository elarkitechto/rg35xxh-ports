#!/bin/bash
# PORTMASTER: crossyroad.zip, Crossy Road.sh
# Testing candidate 0.2.0-rc1. Only RG35XX-H/KNULLI is currently verified.
PORTDIR="$(CDPATH= cd -- "$(dirname -- "$0")/crossyroad" && pwd)" || exit 1
export XDG_DATA_HOME="${XDG_DATA_HOME:-$HOME/.local/share}"
if [ -n "${CROSSY_PORTMASTER_DIR:-}" ]; then
 controlfolder="$CROSSY_PORTMASTER_DIR"
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
 echo "Crossy Road requires PortMaster. Please install or update it." >&2
 exit 1
fi
# PortMaster's scripts do not assume errexit/nounset.
source "$controlfolder/control.txt"
case "${CFW_NAME:-}" in KNULLI|knulli) crossy_knulli=1;; *) crossy_knulli=0;; esac
mod_script="$controlfolder/mod_${CFW_NAME:-}.txt"
if [ ! -f "$mod_script" ] && [ "$crossy_knulli" = 1 ]; then mod_script="$controlfolder/mod_KNULLI.txt"; fi
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
 echo "Crossy Road: $*"
 if declare -F pm_message >/dev/null; then pm_message "Crossy Road: $*"; fi
}
error_message() { message "$@"; sleep "${CROSSY_ERROR_DELAY:-6}"; }
case "${DEVICE_ARCH:-$(uname -m)}" in aarch64|arm64) ;; *) error_message 'This candidate requires ARM64.'; exit 1;; esac
export CROSSY_RUNTIME_DIR="$PORTDIR/runtime"
# Keep the existing private KNULLI installation's saves in place. Fresh installs
# and other systems use self-contained port data; no automatic save copying.
if [ -z "${CROSSY_DATA_DIR:-}" ]; then
 if [ "$crossy_knulli" = 1 ] && [ -d /userdata/system/cr/prefs ]; then
  export CROSSY_DATA_DIR=/userdata/system/cr
 else
  export CROSSY_DATA_DIR="$PORTDIR/gamedata"
 fi
fi
export CROSSY_APK_PATH="$PORTDIR/crossy-road-4-5-0.apk"
mkdir -p "$CROSSY_RUNTIME_DIR" || exit 1
export LD_LIBRARY_PATH="$CROSSY_RUNTIME_DIR${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
if ! sh "$PORTDIR/install.sh" "$PORTDIR" > "$PORTDIR/install.log" 2>&1; then
 cat "$PORTDIR/install.log" >&2
 error_message "Setup failed. Check crossyroad/install.log; supply the exact Crossy Road 4.5.0 APK and launch again."
 exit 1
fi
# Use PortMaster's controller database; preserve a supplied mapping if its
# get_controls implementation uses SDL_GAMECONTROLLERCONFIG_FILE instead.
[ -z "${sdl_controllerconfig:-}" ] || export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
# KNULLI's tested SDL backend. Other backends remain experimental, not forced.
if [ "$crossy_knulli" = 1 ]; then export SDL_VIDEODRIVER=mali; fi
if command -v ldd >/dev/null && ldd "$CROSSY_RUNTIME_DIR/crossy-loader" 2>&1 | grep -q 'not found'; then
 ldd "$CROSSY_RUNTIME_DIR/crossy-loader" > "$PORTDIR/runtime-dependencies.log" 2>&1
 error_message 'Missing runtime libraries. This candidate is verified only on RG35XX-H/KNULLI; see runtime-dependencies.log.'
 exit 1
fi
cd "$CROSSY_RUNTIME_DIR" || exit 1
export HK_PTHREAD_SHIM=1 HK_SDLPAD=1 HK_FRAMES=0
export CROSSY_NATIVE_INPUT=1 CROSSY_FIX_INPUT_EVENT=1 CROSSY_FORCE_KEYBOARD=1
export CROSSY_SKIP_WAIT=1 CROSSY_SKIP_ANALYTICS=1 CROSSY_SKIP_BILLING=1
export CROSSY_SKIP_ANDROID_TIME=1 CROSSY_FIX_READLINK=1 CROSSY_DIAG_SKIP_AGE_GATE=1
export CROSSY_SYNC_SAVE=1 CROSSY_FORCE_OPENSL=1 CROSSY_AUDIO_SDK=25 CROSSY_AUDIO_PROPERTIES=1
export CROSSY_PULSE_DIRECT=1 CROSSY_HAPTICS=1 CROSSY_HAPTIC_ENVIRONMENT=1
unset CROSSY_CAPTURE_DIR CROSSY_TEST_SAVE_ON_LIMIT CROSSY_TEST_PAUSE_HUD CROSSY_SECONDS CROSSY_PREFS_DIR CROSSY_TRACE_INPUT CROSSY_TRACE_AUDIO CROSSY_TRACE_EGL
unset CROSSY_TEST_OPTIONS_OPEN CROSSY_TEST_OPTIONS_NAV CROSSY_TEST_MENU_ACTION CROSSY_FORCE_CONTROLLER_UI CROSSY_TEST_RESTART
unset CROSSY_AUTOPRESS_UP CROSSY_AUTOPAUSE_DELAY CROSSY_AUTORESUME_DELAY CROSSY_TEST_WELCOME_CONFIRM CROSSY_TEST_EXIT_SAVE
unset CROSSY_TEST_PRIZE_OPEN CROSSY_TEST_FORCE_START_UI CROSSY_TRACE_SAVE CROSSY_TEST_FREE_GIFT_BUTTON CROSSY_TEST_FREE_GIFT_READY
unset CROSSY_TEST_GIFT_ROUTE CROSSY_TEST_GROUP_NAV CROSSY_TEST_CHARACTER_STEP CROSSY_TRACE_SDL_BUTTONS CROSSY_PAD_BRIDGE CROSSY_DIRECT_GAME_INPUT
ulimit -c 0
if declare -F pm_platform_helper >/dev/null; then pm_platform_helper "$CROSSY_RUNTIME_DIR/crossy-loader"; fi
printf '%s\n' "Crossy Road 0.2.0-rc1" "CFW=${CFW_NAME:-unknown} ARCH=${DEVICE_ARCH:-unknown}" "DATA=$CROSSY_DATA_DIR" "RUNTIME=$CROSSY_RUNTIME_DIR" "SDL_BACKEND=${SDL_VIDEODRIVER:-automatic}" > "$PORTDIR/log.txt"
LD_PRELOAD="$CROSSY_RUNTIME_DIR/libcrossy-jni-native.so" ./crossy-loader >> "$PORTDIR/log.txt" 2>&1 &
child_pid=$!
wait "$child_pid"
status=$?
child_pid=
if [ "$status" -ne 0 ]; then error_message "Game stopped (exit $status). Please attach crossyroad/log.txt to your test report."; fi
exit "$status"

#!/bin/bash
# PORTMASTER: altosadventure.zip, Alto's Adventure.sh
XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}
if [ -n "${ALTO_PORTMASTER_DIR:-}" ]; then
  controlfolder="${ALTO_PORTMASTER_DIR}"
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
[ -f "$controlfolder/control.txt" ] || { echo "Alto's Adventure requires PortMaster." >&2; exit 1; }
source "$controlfolder/control.txt"
[ ! -f "$controlfolder/mod_${CFW_NAME}.txt" ] || source "$controlfolder/mod_${CFW_NAME}.txt"
get_controls

PORTDIR="$(CDPATH= cd -- "$(dirname -- "$0")/altosadventure" && pwd)" || exit 1
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
if [ -z "${ALTO_DATA_DIR:-}" ]; then
  if [ -d /userdata/system/alto/prefs ]; then export ALTO_DATA_DIR=/userdata/system/alto;
  else export ALTO_DATA_DIR="$PORTDIR/gamedata"; fi
fi
export ALTO_RUNTIME_DIR="$RUNTIME" ALTO_APK_PATH="$PORTDIR/altos-adventure-1-8-8.apk"
if ! sh "$PORTDIR/install-data" "$PORTDIR" > "$PORTDIR/install.log" 2>&1; then
  cat "$PORTDIR/install.log" >&2
  pm_message 'Game setup failed. Check install.log and the supported game-file version.'
  exit 1
fi
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
case "$CFW_NAME" in KNULLI|knulli) export SDL_VIDEODRIVER=mali ;; esac
export LD_LIBRARY_PATH="$RUNTIME:/usr/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export HK_PTHREAD_SHIM=1 HK_NOSPOOF=1 HK_SDLPAD=1
export ALTO_FRAMES=0 ALTO_SECONDS=0 ALTO_OPENSL=1 CROSSY_PULSE_DIRECT=1
unset ALTO_OFFLINE_UI_SMOKE ALTO_DIALOG_SMOKE ALTO_AUTOPRESS ALTO_SETTINGS_SMOKE ALTO_UI_SMOKE ALTO_HAPTIC_TEST ALTO_CAPTURE ALTO_LEGACY_PACING ALTO_PREFS_DIR
ulimit -c 0
cd "$RUNTIME" || exit 1
pm_platform_helper "$RUNTIME/alto-loader"
./alto-loader > "$PORTDIR/log.txt" 2>&1 &
child_pid=$!
wait "$child_pid"
status=$?
child_pid=
[ "$status" -eq 0 ] || pm_message "Game stopped (exit $status). Check altosadventure/log.txt."
exit "$status"

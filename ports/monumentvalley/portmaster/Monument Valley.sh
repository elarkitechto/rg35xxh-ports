#!/bin/bash
# PORTMASTER: monumentvalley.zip, Monument Valley.sh
XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}
if [ -n "${MONUMENT_PORTMASTER_DIR:-}" ]; then
  controlfolder="${MONUMENT_PORTMASTER_DIR}"
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
[ -f "$controlfolder/control.txt" ] || { echo "Monument Valley requires PortMaster." >&2; exit 1; }
source "$controlfolder/control.txt"
[ ! -f "$controlfolder/mod_${CFW_NAME}.txt" ] || source "$controlfolder/mod_${CFW_NAME}.txt"
get_controls

PORTDIR="$(CDPATH= cd -- "$(dirname -- "$0")/monumentvalley" && pwd)" || exit 1
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
SAVE="${MONUMENT_SAVE_DIR:-/userdata/system/monument-valley/saves}"
mkdir -p "$SAVE"
export MONUMENT_DATA_DIR="$PORTDIR/gamedata" MONUMENT_SAVE_DIR="$SAVE" MONUMENT_PREFS_DIR="$SAVE/prefs"
export MONUMENT_APK_PATH="$PORTDIR/monument-valley-3.10.114.apk" MONUMENT_RUNTIME_DIR="$RUNTIME"
if ! (cd "$RUNTIME" && sha256sum -c runtime.sha256) > "$PORTDIR/install.log" 2>&1; then
  pm_message 'Port runtime is incomplete or corrupted. Reinstall monumentvalley.zip.'; exit 1
fi
if ! sh "$PORTDIR/install-data" "$PORTDIR" > "$PORTDIR/install.log" 2>&1; then
  cat "$PORTDIR/install.log" >&2
  pm_message 'Game setup failed. Check install.log and the supported game-file version.'
  exit 1
fi
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
case "$CFW_NAME" in KNULLI|knulli) export SDL_VIDEODRIVER=mali SDL_AUDIODRIVER=pulseaudio EGL_PLATFORM=gbm MONUMENT_PULSE_DIRECT=1 ;; esac
export LD_LIBRARY_PATH="$RUNTIME:/usr/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"
export MONUMENT_SDLPAD=1 MONUMENT_GLES3=1 MONUMENT_NATIVE_GAMEPAD=1 MONUMENT_LANDSCAPE=1 MONUMENT_FIXED_PAD=1 HK_PTHREAD_SHIM=1 HK_NOSPOOF=1
export MONUMENT_PULSE_BUFFER_MS=220 MONUMENT_AUDIO_RESERVE_MS=120
unset MONUMENT_TATE MONUMENT_FRAMES MONUMENT_SECONDS LD_PRELOAD
ulimit -c 0
cd "$RUNTIME" || exit 1
pm_platform_helper "$RUNTIME/monument-loader"
./monument-loader > "$PORTDIR/log.txt" 2>&1 &
child_pid=$!
wait "$child_pid"
status=$?
child_pid=
[ "$status" -eq 0 ] || pm_message "Game stopped (exit $status). Check monumentvalley/log.txt."
exit "$status"

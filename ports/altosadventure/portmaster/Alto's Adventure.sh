#!/bin/bash
# PORTMASTER: altosadventure.zip, Alto's Adventure.sh
set -eu
PORTDIR="$(CDPATH= cd -- "$(dirname -- "$0")/altosadventure" && pwd)"
LAB=/userdata/system/alto-lab
if ! sh "$PORTDIR/install.sh" "$PORTDIR" > "$PORTDIR/install.log" 2>&1; then
 cat "$PORTDIR/install.log" >&2
 if [ -w /dev/tty0 ]; then
  printf '\033cAlto\047s Adventure: installation failed.\n' > /dev/tty0
  cat "$PORTDIR/install.log" > /dev/tty0
  sleep 8
 fi
 exit 1
fi
cd "$LAB"
export LD_LIBRARY_PATH="$LAB:/usr/lib"
export SDL_VIDEODRIVER=mali
export SDL_GAMECONTROLLERCONFIG='19000000010000000100000000010000,RG35XX-H,a:b4,b:b5,x:b6,y:b3,back:b9,start:b10,guide:b11,leftshoulder:b7,rightshoulder:b8,lefttrigger:b13,righttrigger:b14,dpup:h0.1,dpright:h0.2,dpdown:h0.4,dpleft:h0.8,leftx:a0,lefty:a1,rightx:a2,righty:a3,platform:Linux,'
export HK_PTHREAD_SHIM=1 HK_NOSPOOF=1 HK_SDLPAD=1
export ALTO_FRAMES=0 ALTO_SECONDS=0
export ALTO_OPENSL=1 CROSSY_PULSE_DIRECT=1
unset ALTO_AUTOPRESS ALTO_SETTINGS_SMOKE ALTO_UI_SMOKE ALTO_HAPTIC_TEST ALTO_CAPTURE ALTO_LEGACY_PACING ALTO_PREFS_DIR
ulimit -c 0
./alto-loader > "$LAB/play.log" 2>&1

#!/bin/bash
# PORTMASTER: crossyroad.zip, Crossy Road.sh
set -eu

PORTDIR="$(CDPATH= cd -- "$(dirname -- "$0")/crossyroad" && pwd)"
SYSTEM_ROOT="${CROSSY_SYSTEM_ROOT:-/userdata/system}"
LAB="$SYSTEM_ROOT/crossyroad-lab"

if ! sh "$PORTDIR/install.sh" "$PORTDIR" "$SYSTEM_ROOT" > "$PORTDIR/install.log" 2>&1; then
  cat "$PORTDIR/install.log" >&2
  if [ -w /dev/tty0 ]; then
    printf '\033cCrossy Road: installation failed.\n' > /dev/tty0
    cat "$PORTDIR/install.log" > /dev/tty0
    sleep 6
  fi
  exit 1
fi

# Preserve the controller path and environment verified on RG35XX-H / KNULLI.
MAPPING='19000000010000000100000000010000,RG35XX-H,a:b4,b:b5,x:b6,y:b3,back:b9,start:b10,guide:b11,leftshoulder:b7,rightshoulder:b8,lefttrigger:b13,righttrigger:b14,dpup:h0.1,dpright:h0.2,dpdown:h0.4,dpleft:h0.8,leftx:a0,lefty:a1,rightx:a2,righty:a3,platform:Linux,'
cd "$LAB"
unset CROSSY_TEST_OPTIONS_OPEN CROSSY_TEST_OPTIONS_NAV CROSSY_TEST_MENU_ACTION CROSSY_FORCE_CONTROLLER_UI CROSSY_TEST_RESTART CROSSY_AUTOPRESS_UP CROSSY_AUTOPAUSE_DELAY CROSSY_AUTORESUME_DELAY
unset CROSSY_TEST_WELCOME_CONFIRM CROSSY_TEST_EXIT_SAVE CROSSY_TEST_PRIZE_OPEN CROSSY_TEST_FORCE_START_UI CROSSY_TRACE_SAVE
unset CROSSY_TEST_FREE_GIFT_BUTTON CROSSY_TEST_FREE_GIFT_READY CROSSY_TEST_GIFT_ROUTE
unset CROSSY_TEST_GROUP_NAV CROSSY_TEST_CHARACTER_STEP
unset CROSSY_TRACE_INPUT CROSSY_TRACE_SDL_BUTTONS CROSSY_PAD_BRIDGE CROSSY_DIRECT_GAME_INPUT
export SDL_GAMECONTROLLERCONFIG="$MAPPING"
export LD_LIBRARY_PATH="$LAB:/usr/lib"
export LD_PRELOAD="$LAB/libcrossy-jni-native.so"
export SDL_VIDEODRIVER=mali
export NODRIVER=1 SWAPPY_NOPATCH=1 HK_NOTRAP=1
export CROSSY_SKIP_WAIT=1 CROSSY_SKIP_ANALYTICS=1 CROSSY_SKIP_BILLING=1
export CROSSY_SKIP_ANDROID_TIME=1 CROSSY_FIX_READLINK=1
export CROSSY_FORCE_KEYBOARD=1 CROSSY_NATIVE_INPUT=1 CROSSY_FIX_INPUT_EVENT=1 HK_SDLPAD=1
export CROSSY_DIAG_SKIP_AGE_GATE=1
export CROSSY_SYNC_SAVE=1
export CROSSY_FORCE_OPENSL=1 CROSSY_AUDIO_SDK=25 CROSSY_AUDIO_PROPERTIES=1
export CROSSY_PULSE_DIRECT=1
export HK_FRAMES=0
./crossy-loader-sdl-single > "$LAB/crossy-road.log" 2>&1

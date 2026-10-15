#!/bin/bash
# Prototype launcher: keep KNULLI's patched SDL2 and graphics environment.
XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}
for candidate in /opt/system/Tools/PortMaster /opt/tools/PortMaster "$XDG_DATA_HOME/PortMaster" /roms/ports/PortMaster /userdata/roms/ports/PortMaster; do
    if [ -f "$candidate/control.txt" ]; then controlfolder="$candidate"; break; fi
done
if [ -n "$controlfolder" ]; then
    source "$controlfolder/control.txt"
    [ -f "$controlfolder/mod_${CFW_NAME}.txt" ] && source "$controlfolder/mod_${CFW_NAME}.txt"
    get_controls
fi
GAMEDIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)/threes-knulli"
cd "$GAMEDIR" || exit 1
mkdir -p saves
export THREES_SAVE_DIR="$GAMEDIR/saves"
export THREES_FULLSCREEN=1
export SDL_GAMECONTROLLERCONFIG="${sdl_controllerconfig:-$SDL_GAMECONTROLLERCONFIG}"
# Native SDL input handles both sticks and F+Start; avoid a second key mapper.
# PortMaster still supplies the controller mapping and system environment.
finish() {
    if declare -F pm_finish >/dev/null; then pm_finish; fi
}
trap finish EXIT
./threes > "$GAMEDIR/log.txt" 2>&1
exit $?

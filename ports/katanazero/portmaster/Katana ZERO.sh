#!/bin/bash
XDG_DATA_HOME=${XDG_DATA_HOME:-$HOME/.local/share}
for candidate in /opt/system/Tools/PortMaster /opt/tools/PortMaster "$XDG_DATA_HOME/PortMaster" /roms/ports/PortMaster; do
 if [ -f "$candidate/control.txt" ]; then controlfolder="$candidate"; break; fi
done
[ -n "$controlfolder" ] || exit 1
source "$controlfolder/control.txt"
[ -f "$controlfolder/mod_${CFW_NAME}.txt" ] && source "$controlfolder/mod_${CFW_NAME}.txt"
get_controls
GAMEDIR="/$directory/ports/katanazero"
cd "$GAMEDIR" || exit 1
exec > >(tee "$GAMEDIR/log.txt") 2>&1
python3 "$GAMEDIR/prepare.py" >> "$GAMEDIR/install.log" 2>&1 || { pm_finish; exit 1; }
export LD_LIBRARY_PATH="/usr/lib:$GAMEDIR/lib:${LD_LIBRARY_PATH:-}"
export SDL_GAMECONTROLLERCONFIG="$sdl_controllerconfig"
export KATANA_UNLOCKED_COMPAT=1
export KATANA_LOCAL_SAVE_DIR="$GAMEDIR/save/local-slots"
chmod +x ./gmloadernext.aarch64
$GPTOKEYB "gmloadernext.aarch64" -c "$GAMEDIR/katana.gptk" &
pm_platform_helper "$GAMEDIR/gmloadernext.aarch64"
./gmloadernext.aarch64 -c "$GAMEDIR/gmloader.json"
rc=$?
printf 'KATANA_EXIT=%s\n' "$rc"
pm_finish
exit "$rc"

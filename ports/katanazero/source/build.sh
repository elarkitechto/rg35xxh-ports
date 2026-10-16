#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
work=${KATANA_BUILD_DIR:-$root/build}
revision=522d964025e8ecb739c5aeb0f57afaf6f7927849
if [ ! -d "$work/.git" ]; then
 git clone --recursive https://github.com/JohnnyonFlame/gmloader-next.git "$work"
fi
[ "$(git -C "$work" rev-parse HEAD)" = "$revision" ] || git -C "$work" checkout "$revision"
git -C "$work" submodule update --init --recursive
if git -C "$work" apply --check "$root/source/gmloader-katana.patch"; then
 git -C "$work" apply "$root/source/gmloader-katana.patch"
else
 git -C "$work" apply --reverse --check "$root/source/gmloader-katana.patch"
fi
cd "$work"
make -f Makefile.gmloader -j"${JOBS:-4}" "$@"

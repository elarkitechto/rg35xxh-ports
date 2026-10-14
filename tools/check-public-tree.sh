#!/bin/sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
cd "$root"
bad=0
git ls-files | while IFS= read -r path; do
  case "$path" in
    *.apk|*.zip|*.7z|*.so|*.dat|*.sav|*.png|*.jpg|*.jpeg|*/userdata/*|*/prefs/*|*/assets/*|.local/*)
      echo "Private or binary file staged: $path" >&2; exit 1;;
  esac
  size=$(wc -c < "$path")
  if [ "$size" -gt 1048576 ]; then echo "File over 1 MiB: $path" >&2; exit 1; fi
done || bad=1
if git grep -nIE '(/Volumes/|192\.168\.[0-9]+\.[0-9]+|(password|passwd|token|secret)[[:space:]]*[:=])' -- ':!tools/check-public-tree.sh' >/dev/null; then
  echo 'Potential local path, network address or secret in tracked text:' >&2
  git grep -nIE '(/Volumes/|192\.168\.[0-9]+\.[0-9]+|(password|passwd|token|secret)[[:space:]]*[:=])' -- ':!tools/check-public-tree.sh' >&2
  bad=1
fi
[ "$bad" -eq 0 ]

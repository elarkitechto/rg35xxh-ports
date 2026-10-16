#!/usr/bin/env python3
import argparse, zipfile
from pathlib import Path
p=argparse.ArgumentParser()
p.add_argument('--loader',type=Path,required=True)
p.add_argument('--libm',type=Path,required=True)
p.add_argument('--cover',type=Path,required=True)
p.add_argument('--screenshot',type=Path,required=True)
p.add_argument('--output',type=Path,required=True)
a=p.parse_args();root=Path(__file__).resolve().parents[1]
a.output.parent.mkdir(parents=True,exist_ok=True)
with zipfile.ZipFile(a.output,'w',zipfile.ZIP_DEFLATED) as z:
 for file in sorted((root/'portmaster').rglob('*')):
  if file.is_file(): z.write(file,file.relative_to(root/'portmaster'))
 z.write(a.cover,'cover.png')
 z.write(a.cover,'katanazero/cover.png')
 z.write(a.screenshot,'screenshot.png')
 z.write(a.loader,'katanazero/gmloadernext.aarch64')
 z.write(a.libm,'katanazero/lib/arm64-v8a/libm.so')
 z.write(root/'LICENSE','katanazero/LICENSE')
 z.write(root/'source/gmloader-katana.patch','katanazero/source/gmloader-katana.patch')
 z.write(root/'source/README.md','katanazero/source/README.md')
 z.write(root/'source/build.sh','katanazero/source/build.sh')
print(a.output)

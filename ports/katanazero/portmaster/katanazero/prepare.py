#!/usr/bin/env python3
"""Prepare user-provided Katana ZERO 1.1.10 data; never download game files."""
import hashlib, io, json, os, struct, sys, tempfile, zipfile, zlib
from pathlib import Path
ROOT = Path(__file__).resolve().parent
EXPECTED = {
 'lib/arm64-v8a/libyoyo.so': 'a7f8f231cc216dfc9c93d9ac61ba4f082a2332712528be0106234887898ef127',
 'assets/assets/game.droid': '0b997cb57c6907413d1c5d94529698cd5a27cfde31e618b61743e5dc0d6accca',
 'assets/assets/options.ini': '7f40db42ddf6ebe3cc3d2b11cf15b94656a89c5db6887beb059a1c5aaa5a6a36',
}
def main():
 source = ROOT / 'katana-zero-1.1.10.xapk'
 if not source.is_file():
  raise RuntimeError('Copy your Katana ZERO Netflix 1.1.10 XAPK here as katana-zero-1.1.10.xapk.')
 digest=hashlib.sha256(source.read_bytes()).hexdigest()
 marker=ROOT/'prepared.json'
 if marker.exists() and json.loads(marker.read_text()).get('source_sha256')==digest and (ROOT/'katana.port').exists(): return
 with zipfile.ZipFile(source) as outer:
  manifest=json.loads(outer.read('manifest.json'))
  if manifest.get('package_name')!='com.netflix.NGP.KatanaZero' or manifest.get('version_name')!='1.1.10':
   raise RuntimeError('Unsupported game version; requires Katana ZERO Netflix 1.1.10 ARM64.')
  found={}
  for split in ['config.arm64_v8a.apk','DynamicAssetDeliveryFiles.apk']:
   with zipfile.ZipFile(io.BytesIO(outer.read(split))) as archive:
    for name in archive.namelist():
     if name in EXPECTED or (name.startswith('assets/assets/') and not name.endswith('/')):
      found[name]=archive.read(name)
  for name,wanted in EXPECTED.items():
   if hashlib.sha256(found[name]).hexdigest()!=wanted: raise RuntimeError('Unsupported game component: '+name)
  with tempfile.TemporaryDirectory(dir=ROOT) as tmp:
   stage=Path(tmp)
   with zipfile.ZipFile(stage/'katana.port','w',zipfile.ZIP_STORED) as target:
    for name,data in found.items():
     if name.startswith('assets/assets/'): target.writestr(name[7:],data)
   for name in ['game.droid','options.ini']:
    (stage/name).write_bytes(found['assets/assets/'+name])
   (ROOT/'lib/arm64-v8a').mkdir(parents=True,exist_ok=True)
   (ROOT/'save').mkdir(exist_ok=True)
   (ROOT/'lib/arm64-v8a/libyoyo.so').write_bytes(found['lib/arm64-v8a/libyoyo.so'])
   os.replace(stage/'katana.port',ROOT/'katana.port')
   for name in ['game.droid','options.ini']: os.replace(stage/name,ROOT/'save'/name)
  def chunk(kind,data): return struct.pack('>I',len(data))+kind+data+struct.pack('>I',zlib.crc32(kind+data)&0xffffffff)
  (ROOT/'save/splash.png').write_bytes(b'\x89PNG\r\n\x1a\n'+chunk(b'IHDR',struct.pack('>IIBBBBB',1,1,8,2,0,0,0))+chunk(b'IDAT',zlib.compress(b'\0\0\0\0'))+chunk(b'IEND',b''))
  marker.write_text(json.dumps({'source_sha256':digest,'version':'1.1.10'}))
if __name__=='__main__':
 try: main()
 except Exception as error: print('Setup failed:',error,file=sys.stderr);sys.exit(1)

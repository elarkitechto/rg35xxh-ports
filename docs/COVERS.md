# KNULLI port covers

This collection uses **vertical 2D box art**. The handheld’s 640×480 display and
PortMaster’s landscape screenshot do not determine the cover format.

## Specifications

- Use front-facing vertical artwork with a readable title. Prefer 2:3, such as 600×900. Preserve suitable original proportions rather than stretching. A previous Threes! cover at 528×704 was also accepted.
- Keep `screenshot.png` separate: actual gameplay in 4:3, at least 640×480. Do not use a landscape screenshot as the collection’s cover.
- Use artwork for the exact game. Do not substitute Alto’s Odyssey or The Alto Collection for Alto’s Adventure. If necessary, compose vertical artwork from the game’s own assets and inspect it visually.
- Keep titles, characters and major elements intact. Avoid cropping or adding bands solely to force an aspect ratio. Use PNG or JPEG as specified by metadata; package builders use PNG.
- In packages, `gameinfo.xml` points both `<image>` and `<thumbnail>` to `./<port>/cover.png`.
- On the handheld, store covers in `roms/ports/images/boxart/<port>.png`; point both image and thumbnail entries in `gamelist.xml` to that file.
- Back up metadata and preserve launchers, saves and play statistics. Refresh EmulationStation outside an active game session.

## Verification

1. Open and inspect the cover, including its title and composition.
2. Verify image and thumbnail paths in the package and on the handheld.
3. Compare SHA-256 hashes of the installed and packaged covers.
4. Record artwork provenance, dimensions and hash.

`tools/validate-port-media.py` rejects landscape/square covers, screenshots below
640×480 or outside 4:3, and identical cover/screenshot files. Visual inspection
is still required.

## Alto’s Adventure

Selected artwork: [LaunchBox Android front cover](https://gamesdb.launchbox-app.com/games/images/101151-altos-adventure),
[original PNG](https://images.launchbox-app.com/3583a3ee-f75a-4eeb-8563-09b0695d8b7f.png),
654×1024, preserved without distortion. Gameplay screenshot: 640×480.

Cover SHA-256: `68c67d8974fadc30b8642c1cae8a48b3296d46cd6e9d3154092fc20f4aaf4b45`.

## Crossy Road

The candidate uses the collection’s previously approved vertical cover,
1080×1683, visually inspected without distortion. Confirm its original
provenance before redistributing it. Gameplay screenshot: 640×480.
Package image and thumbnail: `./crossyroad/cover.png`.

Cover SHA-256: `227c1360c3893d164fc979f50aa0996368872c782cad3534475b1a5c01f61ed7`.

## Threes! and Triples

Threes! uses the former demake cover from `images/boxart/threes.jpg`, 300×450,
converted to PNG without resizing. Its original provenance was not recorded.
Triples uses an ImageGen title edit of that composition, 1024×1536, reading
TRIPLES. Both are vertical 2:3 and were visually inspected.

Local artwork records and hashes are in `.local/threes-media/ARTWORK.md` and
`.local/threes-media/artwork.sha256`. Handheld image and thumbnail paths are
`./images/boxart/threes.png` and `./images/boxart/triples.png` respectively.
Threes! packages use `threes/` and preserve the existing system save path.
Triples keeps `threes-knulli/` on the device for save compatibility.

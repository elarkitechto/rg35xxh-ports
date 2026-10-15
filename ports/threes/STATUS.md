# Threes! Android port status

Current candidate: **full 3.1.6144 ARM64**, Unity 2022.3.26f1.
The sections below retain the earlier Free-edition investigation history.


This is the original Android game, independent of the Triples demake in
`ports/triples`. Crossy Road and Alto remain unchanged.

## Candidate

Threes! Free 3.1 (24576), Unity 2022.3.26f1, ARM64 IL2CPP.
XAPK SHA-256: `99dcba5f8dbcbc4294507c6b30d9f0719ae4127228186e9a471ecdb45a77c880`.
Game-specific hooks and expected opcodes are isolated in `threes_game_patches.h`
and `threes_profile.c`. Proprietary libraries and assets stay outside Git.

## Device evidence — 2026-10-15

- Original engine and localized tutorial render on RG35XX-H / KNULLI.
- A 1,800-frame run completed normally, including original SaveGameForReal.
- Full-panel TATE: logical 480×640, rotated onto physical 640×480.
  Landscape now fits the same complete portrait surface to 360×480, centered
  with side bars. Unity keeps a stable 480×640 backbuffer; presentation changes
  no longer trigger a mismatched Android surface resize.
- SDL recognizes the built-in RG35XX-H controller using the Crossy Road mapping.
  Input feeds a real Unity Input System Gamepad through native StateEvents.
- Synthetic events through that same queue completed the original tutorial
  and reached the score screen (312 points) in a 4,000-frame run ending normally.
  They move cards and merge numbers. Digital directions also feed leftStick,
  because the original Move action uses that axis.
- OpenSL connects to PulseAudio at 48 kHz stereo; nonzero PCM peaks and zero
  reported underruns were logged in the initial run; a later reload reported
  one underrun. Audible output still needs a listening check.
- A restart without synthetic input restored the saved 312-point score screen
  instead of the tutorial. The CompletedTutorial flag and board keys survived.
- Android omits MenuNavigation.Update; an added reflection bridge navigates
  live Menu/SimpleButton objects and invokes their original OnPress methods.
  NameMenu.Done accepts the default signature without an Android keyboard.
  Device events accepted the signature, navigated results and activated Retry
  to start another game. Share/Tweet/GC buttons are hidden from this offline
  interface; credit and purchase rules remain intact.
- SharedPreferences keys and string values persist to isolated files, with
  atomic replacement, a background synchronization every two seconds, and a
  joined final flush on normal exit. Unchanged values avoid redundant flash
  writes. Abrupt power loss can lose the most recent unsynchronized changes.
  Verbose JNI file logging is opt-in with THREES_DEBUG.
- Fresh installation, repeat installation, library hashes and save-file
  preservation passed local tests. Missing and incorrect XAPKs are rejected
  before modifying installed data. The preview also installed successfully
  under the device ports folder.
- Native-decoded values matched exactly before and after a no-input reload:
  `2 12 2 3 6 2 6 1 1 1 24 3 3 1 3 12`. Normal shutdown flushed preferences
  and exited with status zero.

- The installed PortMaster launcher ran with fresh isolated saves and the
  bundled runtime. A bounded SIGTERM test invoked original SaveGameForReal,
  flushed preferences and ended the render loop; EmulationStation was restored.

## Controller/display/haptic revision — 2026-10-15

- Face buttons are directions only during TATE gameplay; menus always use
  labelled A to confirm and B to cancel/back. SDL and native Unity Gamepad
  events are retained. Held inputs must return to neutral after a context or
  orientation change, preventing a menu press from spilling into a board move.
- UI navigation includes visible parent toolbar buttons and dispatches each
  action to its original owning Menu, including Back on the Options screen.
- Rumble follows original numMoves, score/lastHighestMerge, didNope and LOST
  transitions, rather than raw controller presses. SDL accepted movement pulses
  with result zero on the console. Physical strength still needs user feedback.
- A physical-button mapping diagnostic uses the production mapper; menu A/B
  produced only South/East button bits and neutral axes in TATE.
- The 1,200-frame regression switched to landscape at frame 600 and back to
  TATE at frame 900. Captures at 750 and 1200 show the complete board in both
  orientations. Original save and final preference flush completed, exit zero.
- Movement and merge pulses both returned SDL result zero. An input replay
  verified menu A/B axes remain neutral, Back returns from Options to MidGame,
  and a held B is suppressed after closing the menu until released.
- Rebuilt and installed as the independent Threes! Android preview; saves stay
  in the existing game-specific directory.

## Remaining validation

Manually verify physical buttons, both sticks, audible output, and remaining
menus. Ordinary gameplay and an unfinished board have rendered after restart.
The native CryptoInt decoder is used to verify saved ranks because original
ciphertext changes on save; ciphertext hashes alone are not a valid board test.
A 4,000-frame diagnostic completed normally. Earlier 45-second watchdogs expired
during active writes; those runs are not counted as successful normal exits.

The archive is a development preview, not a PortMaster submission. Cover art,
submission metadata and broader device compatibility remain to be prepared.

## Catalogue and feedback revision — 2026-10-15

- Public launcher is Threes!.sh; demake launcher is Triples.sh. Runtime and
  save directories retain their existing names to preserve progress.
- Horizontal presentation extends the sampled background to the full panel;
  board and menu artwork retain their aspect ratio. Capture verified.
- Menu L1/R1 and horizontal directions call MoreMasterMenu.SetIndex. Device
  replay switched AppOptions to GameOptions and back, then returned to play.
- Merges compare original Slot value counts rather than record counters.
  Forming 12 or more uses a 62,000-strength 160 ms pulse; regression logged
  12, 24 and another 12, with successful SDL rumble calls.
- The three APK preparers are now named install-data, invoked by the same
  main launchers. KNULLI no longer scans them as shell-game entries. Alto
  and Crossy launcher suites passed (8 tests each).
- Covers are private local media: existing Threes cover transferred to original
  game, generated title variant TRIPLES for the demake. See docs/COVERS.md.

## Daytime cards in night mode investigation — 2026-10-15

Attempted in isolated copies of saves, never deployed to the installed port.
TwoDeeSpriteInternal.SetPalette at 0x87d6a4 and GetTextureIndex at 0x87d294
were inspected with pinned prologue validation. Card atlas indices are 2 normal
and 4 night; the texture enumerations differ after index 410.

Forcing daytime atlas names under AnimTile ancestors preserved the dark board
and changed face artwork, but the card bodies rendered incorrectly. Refreshing
UV/material meshes still produced missing bodies and stretched strips. Merely
switching sprite atlases is therefore not a valid fix for these animation assets.
The experiment was removed from the default runtime; source snapshots and
screenshots are private in .local/threes-night-investigation. A future attempt
should adapt the night atlas resources/animation geometry while preserving their
layout, rather than deploying the failed atlas-selection hooks.

## Full-edition migration — 2026-10-15

- APK package identity: `vo.threes.exclaim`, versionName `3.1.6144`, versionCode 50.
- APK SHA-256: `7d28059f0993285382da108324f844d146b375b64a1600fac4b8e6f5cf3c482a`.
- IL2CPP SHA-256: `a3dff870ae850698a3de67d405728b8bafef42d352d088773cd3e71a3f546a9f`.
- Unity SHA-256: `7f91e8c5d6016e05019098b461690b7d2d45d2e10757959cffa3b78e5d4568fe`.
- Separate guarded full-edition patch table. Relocated Java readiness, Linux
  vsync clock, FMOD output hook and native Input System clock from the pinned
  libraries. Compared relevant managed field layouts against Free; Menu,
  SimpleButton, GameCamera, MoreMasterMenu and Threes match.
- Reused controller navigation, TATE/landscape presentation, haptic feedback,
  offline boundary and save implementation. JNI reports the full package name.
- Original `FreeLogic.IsFree` returns zero. No play-counter, ad-reward or
  purchase-state patch is applied. The earlier Free-limit experiment is excluded.
- The first isolated 1,200-frame device replay loaded a copy of the existing
  board and 16 local scores, activated Retry, moved tiles and logged strong
  feedback for repeated merges of 12. Original SaveGameForReal and preference
  flush completed; exit zero. A real capture shows the complete TATE board.
- OpenSL queued PCM during the replay. This is not a fresh listening test.
- Local fresh installation and repeat installation passed with the exact APK;
  installed library hashes match, and a save sentinel remained unchanged.
- All 20 launcher/setup regression tests passed; source build and complete
  0.2.0-rc1 package generation passed. Owner-supplied APK and libraries are not
  included in the package.
- GDB identified the startup NullReferenceException in
  `LicenseVerification.LoadServiceBinder`, called by `ValidateLicense`.
  The paid profile skips Android LVL initialization at guarded RVA 0x81244c;
  Linux does not provide that Binder service. No license response or purchase
  status is fabricated. A separate caught timezone DirectoryNotFoundException
  uses the original fallback and is unrelated to this issue.
- A temporary early-reflection logging probe crashed before gameplay and was
  discarded; it is not present in the production runtime.

- Final 600-frame reload with the production runtime returned original IsFree=0,
  logged no startup exception, and loaded/saved the exact same 16 board values:
  `3 2 6 2 0 6 2 12 0 0 12 1 1 0 0 1`. Normal exit zero.
- An 800-frame controller replay opened AppOptions, switched to GameOptions
  and back, closed the menu, and changed presentation to landscape while
  retaining a 480x640 logical backbuffer. Exit zero; no logged exception.
- Installed the full 0.2.0-rc1 package under `ports/threes/` and switched
  the existing Threes!.sh entry to that directory. The previous Free runtime
  remains separate. Backed up the prior launcher and the original saves.
- A bounded check of the actual installed PortMaster launcher used copied saves,
  returned original IsFree=0, performed SaveGameForReal and flushed preferences
  before restoring EmulationStation. Installed loader SHA-256:
  `32b3ace9f17f99a4dbf9aee7e522f3a2536545fc8fcd70d66986e758ca856f65`.

## Clean-install audit — 2026-10-15

The 0.2.0-rc2 package was built from source and installed by the official
HarbourMaster CLI on RG35XX-H/KNULLI. An isolated empty-data installation,
second launch and reinstall each reached the diagnostic frame target. The
reinstall preserved a save sentinel and left no extraction staging directory.
These are boot/install checks, not a new manual gameplay or audio assessment.

All three ports passed the same nine-run audit. During final deployment,
451 existing save files were compared byte-for-byte and remained unchanged.
Installed runtime checksums passed. Development directories and duplicate
payloads were removed after verifying private off-device backups. PortMaster
metadata, collection artwork and the Triples demake were preserved.

Launchers now use the standard PortMaster bootstrap and one bundled runtime.
First-run APK/library validation remains necessary for version-specific patches;
normal launches reuse installed data without copying binaries or running ldd.
Diagnostics must clear game-specific environment overrides before restoring
EmulationStation; its final environment was checked.

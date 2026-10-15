# Validation

Tested on RG35XX-H with KNULLI at 640×480 using Android Alto's Adventure 1.8.8 (APK SHA-256 `efb8eb2054e6c66173fc1638e6d005f048b48446b6029c4fdcb8aa8b9dcaaae0`).

- The ARM64 loader and audio bridge build from the public source. The package contains no APK or extracted game libraries.
- Fresh installation and repeated installation both completed. Missing or incorrect APKs were rejected; existing local preferences survived an update.
- Title, tutorial gameplay, menu navigation, Settings sliders and tabs, audio output, save and reload were checked on the device. The final title/audio check ended at its planned timeout, so that run alone does not prove graceful exit.
- A 1500-frame comparison of the old and current frame pacing used separate copied profiles. Measured gameplay samples changed from roughly 28–29 to 47–50 FPS. The scenes differed, so these figures are indicative and do not establish sustained minimum FPS.
- Longer play sessions, physical vibration and other devices or firmware builds have not been validated to the same extent.

The offline policy is documented in [OFFLINE-PATCHES.md](OFFLINE-PATCHES.md). This is an independent port, not an official PortMaster catalogue release.

## Single-button post-upgrade modal fix

The controller bridge previously called ResponsePositive for every DialogBox. That method marks a response but does not start closing a Dismiss dialog; the game's one-button notices need Hide(response). The bridge now distinguishes modal types and routes B/Y to the original closing coroutine for Dismiss. Dialog lookup reads the original singleton field, including while its object is inactive. The normal launcher disables the diagnostic flag.

On-device diagnostic with copied preferences and the original DialogBox.ShowDialog(Dismiss), label Thanks!: confirm shown=0 PASS; back shown=0 PASS. 2300-frame run exited0 and restored EmulationStation. No upgrade was purchased and no coins were changed. The installed loader and package loader hashes match. This verifies that modal type through the same dispatch used by SDL input; the user's exact purchased-upgrade flow still needs a physical retest. See [INPUT.md](INPUT.md).

## Remove Share and rewarded-coin actions from UI

The original GuiSocialShareButton(Action.Share), WatchVideoForCoinsButton and GuiDoubleCoinsButton GameObjects are disabled before collecting controller targets. Scene reactivation is checked on menu entry, periodically in menus and before navigation/confirm. No translated label matching or blanket coin-button filtering is used. Photo-save actions, coin-funded revival and workshop upgrades remain outside this filter.

On-device copied-profile test reached Gameplay5, GameOver7, activated Next, reached EndOfRun9 and StoreEnd13. Logs confirmed hidden Button / Plus Button objects on these screens. The first 2000-frame pass exited0; the expanded 2800-frame pass also exited0 after the post-run/shop sequence and restored EmulationStation. Diagnostic ALTO_OFFLINE_UI_SMOKE advances to the results/shop without purchases; normal launcher unsets it. GameObject.SetActive(false) removes the widget/collider and excludes it from navigation. Physical layout review remains user-verifiable.

## Hide touch pause icon

GuiGameplay.Update pauseButton.SetActive argument changed from true to false at guarded RVA0x6f7234 (original0x320003e1). On-device 1800-frame copied-profile test exited0 and restored the frontend; scripted Start route produced Gameplay5→Pause6→Gameplay5. Frame1500 after resume was visually inspected: the corner pause icon is absent. No per-frame scene search or change to physical pause/resume is required.

## PortMaster candidate 0.2.0-rc1

Launcher now sources installed PortMaster control.txt/CFW mods, calls get_controls, uses its SDL mapping and runs pm_platform_helper/pm_finish. It handles child termination and preserves the loader exit code. Error messages stay visible briefly. No gptokeyb remapping is required for gameplay: the game consumes SDL controller events directly. Only the native Guide+Start exit path is currently verified; other CFW hotkey conventions need tester feedback.

Fresh runtime/data are port-local; JNI assets, prefs, APK and userdata paths take the launcher's explicit environment. Existing KNULLI legacy prefs are selected in place instead of copied. Link/runtime dependencies remain KNULLI-oriented (including libmali.so.0); paths being portable does not establish other-CFW compatibility.

Eight launcher fixture tests passed: controls/helpers/cleanup and paths containing spaces; CFW mod sourcing; fresh portable defaults; loader-error propagation; setup failure; unsupported architecture; missing PortMaster; signal cleanup without leaving a child. Shell syntax, JSON/XML and public-tree checks passed; both ARM64 executables built from source in the one-command package build.

On RG35XX-H/KNULLI 640×480, the candidate launcher installed the pinned APK from scratch into an isolated portable root. An external 55-second test ended137 after the timeout's forced kill during save and was not counted as a graceful exit. A subsequent integration run of the same launcher with only frame/watchdog limits changed (1200/65) exited0, logged CFW=knulli, ARCH=aarch64 and the portable paths, and restored the frontend. No game child remained.

Direct candidate runtime test using the PortMaster-generated SDL mapping and that portable root: 1800frames, Gameplay5, save, exit0; next 1000-frame run loaded llamasSaved=4 and timesJumped=1 and exited0. Nonzero Pulse PCM recorded, no recorded underruns. Reinstallation exited0 and preserved a preference sentinel. Missing/wrong APK each exited1 with an actionable error. These tests used isolated data and did not replace the installed stable port or change the user's saves.

Still pending: physical controller acceptance of the PortMaster-derived mapping, extended sessions, exact purchase/revival regression in this RC, physical vibration and all other CFW/device/resolution combinations. Historical feature tests above apply to the preceding stable revision; do not present them as fresh manual RC tests. Community test draft is prepared but no Discord post or upstream PR has been sent.

## Installed launcher/preparer repair — 2026-10-15

The device had a current launcher paired with an obsolete data preparer.
Alto's old preparer populated the historical lab path while its launcher executed
`ports/altosadventure/runtime/alto-loader`. Crossy's old preparer required a
second positional argument that the current launcher does not supply.

Replaced each installed preparer and matching runtime bundle together, retaining
rollback scripts and all original save directories. The actual Alto launcher
then rendered at least 570 frames in an 18-second bounded boot check using
copied game data. EmulationStation was restored after the check. Timeout exit
124 is expected here; this is a boot check, not a normal-exit or save-persistence
claim. No new physical gameplay/audio acceptance is inferred from these runs.

Launcher tests now include the real production preparer, verify that its runtime
lands where the launcher executes, and reject an incomplete bundle before
launch without changing the save sentinel. Ten tests pass for this port.

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

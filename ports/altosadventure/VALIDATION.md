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

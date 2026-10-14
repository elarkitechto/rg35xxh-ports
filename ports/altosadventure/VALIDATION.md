# Validation

Tested on RG35XX-H with KNULLI at 640×480 using Android Alto's Adventure 1.8.8 (APK SHA-256 `efb8eb2054e6c66173fc1638e6d005f048b48446b6029c4fdcb8aa8b9dcaaae0`).

- The ARM64 loader and audio bridge build from the public source. The package contains no APK or extracted game libraries.
- Fresh installation and repeated installation both completed. Missing or incorrect APKs were rejected; existing local preferences survived an update.
- Title, tutorial gameplay, menu navigation, Settings sliders and tabs, audio output, save and reload were checked on the device. The final title/audio check ended at its planned timeout, so that run alone does not prove graceful exit.
- A 1500-frame comparison of the old and current frame pacing used separate copied profiles. Measured gameplay samples changed from roughly 28–29 to 47–50 FPS. The scenes differed, so these figures are indicative and do not establish sustained minimum FPS.
- Longer play sessions, physical vibration and other devices or firmware builds have not been validated to the same extent.

The offline policy is documented in [OFFLINE-PATCHES.md](OFFLINE-PATCHES.md). This is an independent port, not an official PortMaster catalogue release.

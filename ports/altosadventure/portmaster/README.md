# Alto's Adventure — RG35XX-H / KNULLI

Copy your Alto 1.8.8 APK to `ports/altosadventure/altos-adventure-1-8-8.apk` and launch **Alto's Adventure** from Ports. The launcher verifies, extracts and installs all game data and the compiled runtime automatically, then starts the game. No command line, second launcher, compiler or Crossy Road installation is required on the handheld.

Only the exact APK SHA-256 `efb8eb2054e6c66173fc1638e6d005f048b48446b6029c4fdcb8aa8b9dcaaae0` works. APK not supplied. Wrong/missing APK errors go to `altosadventure/install.log` and the console. Keep saves in `/userdata/system/alto`; runtime/logs are in `/userdata/system/alto-lab`.

B confirms/jumps; hold B for backflips. Y goes back/cancels; Start pauses/resumes; F+Start saves and quits. D-pad navigates; left/right adjusts a selected slider; L/R changes Settings pages. Google Play, ads, purchases and telemetry are disabled. Local progress remains enabled.

Tested only on RG35XX-H with KNULLI at 640×480. Short gameplay samples improved from 28–29 to 47–50 FPS after fixing an extra frame delay; sustained FPS varies. Jump/landing haptics are wired, but physical feedback still needs extended validation.

# Controller and modal routing

Alto reads the physical controller through SDL2 GameController events (`SDL_PollEvent`, button/axis events and the RG35XX-H mapping). Android Unity's original input backend cannot directly read Linux evdev, so the port bridges those events to gameplay getters and the original NGUI widgets. Crossy Road also adapts controller events and uses game-specific button/TouchAction/menu routes; neither port is an unmodified Android controller backend.

## Modal types

The original DialogBox distinguishes Wait=0, Dismiss=1, Confirm=2, RewardedAdPrompt=3 and Verify=4. Treating them all as positive/negative responses causes single-button post-upgrade messages to remain on screen.

- Dismiss (such as Thanks after spending in-game coins): B or Y calls the original `DialogBox.Hide(response)` at 0x6be418. It runs the game's closing coroutine, completes the response, clears the modal state and restores underlying UI. `ResponsePositive` alone only marks the response and does not hide the dialog.
- Confirm: B invokes ResponsePositive; Y invokes ResponseNegative. The caller owns the resulting transaction and closing sequence.
- Wait: no automatic acceptance/cancellation. Consume input while the game finishes its operation.
- Verify: no blanket positive response. Y retains the negative route; verification buttons need their own original callbacks.

The modal consumes input before general menu/gameplay handling. Closing a dismiss modal resets controller focus so a later press returns to the visible interface. Repeated close events are ignored during the original hide coroutine. The DialogBox singleton is read through IL2CPP reflection, rather than relying solely on a search that excludes inactive objects.

`ALTO_DIALOG_SMOKE=1` is a diagnostic only. At Title after frame900 it opens the original Dismiss dialog twice, closes it once using confirm and once using back, and logs whether IsShown becomes false. It does not buy an upgrade or add/spend coins. Run with a copied preference directory, timeout and restoration of EmulationStation. The normal launcher unsets this flag.

## Hidden offline actions

The original scene objects carrying GuiSocialShareButton (Action.Share only), WatchVideoForCoinsButton and GuiDoubleCoinsButton are deactivated with GameObject.SetActive(false) before controller target collection. This removes labels/colliders together and keeps them outside navigation. Identification uses actual component types, independently of display language. Save-photo actions are not matched. Recheck on menu entry, periodically while in menus and before controller actions, because game refreshes may reactivate controls. No extra component enumeration runs during gameplay. Spending earned coins, revival and workshop upgrades are unaffected.

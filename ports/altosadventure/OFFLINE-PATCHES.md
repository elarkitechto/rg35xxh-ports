# Offline adaptation — Alto 1.8.8

This revision disables unavailable Android services while retaining local game
statistics.

## Adapted services

- Unity Analytics: local `enabled:false`, limited tracking and explicit opt-out; event methods return `AnalyticsDisabled`.
- Flurry: sessions and events are disabled; the startup coroutine completes without waiting for remote services.
- GDPR: no geolocation request or consent dialog. The consent getter remains false; user acceptance is not recorded. The service is marked initialized so dependent coroutines do not wait indefinitely.
- AdMob/NoodleNews: retain the advertising singleton, but skip Android mediators and polling. Ads and purchase promotions are hidden.
- Google Billing: retain the purchase singleton and `LoadPurchasedCache`; skip the builder and remote initialization. Purchased/unlocked content getters are unchanged.
- RemoteSettings: skip remote requests and use local game values.
- Networking: game `getaddrinfo` and `socket` calls block external/AF_INET/AF_INET6 destinations, including lookups through `dlsym`. Unix sockets and PulseAudio’s Linux libraries remain available.
- Input: `ChangeInputType` uses Controller (2). Siri Remote (1) navigation is disabled. Original InControl polling is disabled because SDL already delivers game actions.

## Reproducibility

`policy-patches.json` records RVAs and original instructions. `src/alto_policy.c`
checks the instructions before replacement. Patches apply in memory; APKs and
libraries on disk remain unchanged.

The data preparer applies local Analytics configuration before startup. Updates
preserve progress.

## Validation

The 1,600-frame diagnostic covers introduction, title, tutorial gameplay,
message confirmation, PCM audio, saving and exit. Check exceptions, active input
mode and new ArchivedEvents files. Controller mode alone does not establish that
every physical button and menu has been verified.

## Google Play Games

The Google Play invitation belongs to AchievementManager. Manual/automatic
authentication, achievement UI and the consent coroutine are disabled
(`MoveNext=false`); consent/authentication getters return false. Settings hides
`signInButton` (+0x180) and `showGCButtons` (+0x188) through
`GameObject.SetActive(false)`. The table contains 45 guarded patch sites.
Consent is not written and SaveManager is unchanged.

## Touch pause HUD

`GuiGameplay.Update` at 0x6f7234 originally uses `mov w1,#1` (0x320003e1) before
`pauseButton.SetActive`. The guarded replacement `mov w1,wzr` (0x2a1f03e1) keeps
the touch button inactive each frame. SDL Start still switches GameManager
between Gameplay5 and Pause6 directly. No additional gameplay scene enumeration
is needed.

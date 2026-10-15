# Adaptación offline — Alto 1.8.8

Esta revisión desactiva servicios Android que el port no proporciona y conserva las estadísticas locales de la partida.

## Puntos adaptados

- Unity Analytics: configuración local `enabled:false`, tracking limitado y exclusión explícita; métodos de eventos devuelven `AnalyticsDisabled`.
- Flurry: inicio de sesión y eventos desactivados; la coroutine de arranque termina sin esperar servicios remotos.
- GDPR: no se consulta la web de geolocalización ni se abre el diálogo. El getter de consentimiento sigue devolviendo false; no se registra una aceptación del usuario. El servicio se considera inicializado para que otras coroutines no esperen indefinidamente.
- AdMob/NoodleNews: se conserva el singleton de anuncios pero se omiten los mediadores Android y su sondeo; no aparecen anuncios ni promociones de compra.
- Google Billing: se conserva el singleton de compras y se ejecuta `LoadPurchasedCache`; se omiten el builder y la inicialización remota. No se alteran los getters de contenido comprado/desbloqueado.
- RemoteSettings: desactivada la consulta remota; se usan los valores del juego local.
- Red: `getaddrinfo` y `socket` del juego bloquean destinos externos/AF_INET/AF_INET6. También se aplica cuando el juego los resuelve mediante `dlsym`. Los sockets Unix y las bibliotecas Linux de PulseAudio permanecen disponibles.
- Entrada: `ChangeInputType` usa Controller (2); Siri Remote (1) y su navegación no se activan. El sondeo InControl original se desactiva porque el port ya lee SDL y entrega acciones al juego.

## Reproducibilidad

`policy-patches.json` contiene las entradas con RVA e instrucciones originales; `src/alto_policy.c` comprueba las instrucciones antes de reemplazarlas. El APK y las bibliotecas en disco no se modifican: los parches se aplican en memoria.

El instalador aplica la configuración local de Analytics antes del arranque. Los datos de progreso se conservan al actualizar.

## Validación

La revisión se prueba con el mismo recorrido de 1600 frames: introducción, título, partida de tutorial, confirmación de mensajes, audio PCM, guardado y salida. Se revisan excepciones, modo de entrada real y nuevos archivos en ArchivedEvents. La validación física de todos los botones y menús sigue pendiente; el modo Controller no equivale a haber validado todos los menús.

### Google Play Games

La invitación Google Play pertenece a AchievementManager, no a publicidad. Se anulan autenticación manual/automática, UI de logros y coroutine de consentimiento (MoveNext=false), y los getters de consentimiento/autenticación=false. Settings oculta signInButton (+0x180) y showGCButtons (+0x188) mediante GameObject.SetActive(false). La tabla contiene ahora 45 sitios con guards. No se escribe consentimiento, ni se modifica SaveManager.

### Touch pause HUD

GuiGameplay.Update at 0x6f7234 originally has mov w1,#1 (0x320003e1) before pauseButton.SetActive. Guarded replacement mov w1,wzr (0x2a1f03e1) keeps this touch HUD button inactive every frame. SDL Start still changes GameManager Gameplay5/Pause6 directly. No extra gameplay scene enumeration is needed.

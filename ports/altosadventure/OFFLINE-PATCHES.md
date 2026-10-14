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

`policy-patches.json` contiene 37 entradas con RVA e instrucciones originales; `tools/generate-policy-table.py` valida el hash de IL2CPP y genera `src/alto_policy_patches.h`. `src/alto_policy.c` comprueba las instrucciones antes de reemplazarlas. Tres cambios de instrucciones adicionales conservan la caché IAP, evitan la construcción de mediadores y fijan el tipo de mando. El APK y las bibliotecas en disco no se modifican: los parches se aplican en memoria.

`configure-offline.sh` aplica la configuración Analytics antes de arrancar. Los eventos antiguos se movieron a `/userdata/system/alto-lab/analytics-archive.before-offline` como respaldo; no se tocaron los archivos del perfil. El ejecutable anterior está en `alto-loader.before-offline` y el perfil previo en `prefs.before-offline.tar.gz`.

## Validación

La revisión se prueba con el mismo recorrido de 1600 frames: introducción, título, partida de tutorial, confirmación de mensajes, audio PCM, guardado y salida. Se revisan excepciones, modo de entrada real y nuevos archivos en ArchivedEvents. La validación física de todos los botones y menús sigue pendiente; el modo Controller no equivale a haber validado todos los menús.

### Google Play Games

La invitación Google Play pertenece a AchievementManager, no a publicidad. Se anulan autenticación manual/automática, UI de logros y coroutine de consentimiento (MoveNext=false), y los getters de consentimiento/autenticación=false. Settings oculta signInButton (+0x180) y showGCButtons (+0x188) mediante GameObject.SetActive(false). La tabla contiene ahora 45 sitios con guards. No se escribe consentimiento, ni se modifica SaveManager.

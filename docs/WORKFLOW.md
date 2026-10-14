# Flujo de trabajo y entregas

`main` contiene solo fuente y documentación revisadas. Cada port tiene su directorio y su propio estado de compatibilidad. Usa ramas cortas y una solicitud de cambios por port o corrección. Los commits siguen el formato descrito en `CONTRIBUTING.md`.

Antes de cada commit, el hook local y CI ejecutan `tools/check-public-tree.sh`, comprueban sintaxis de scripts y validan JSON/XML. Para activar el hook en otro clon: `git config core.hooksPath .githooks`.

Para preparar una entrega de un port:

1. Confirma la procedencia y licencia de cada archivo que se va a publicar.
2. Prueba instalación desde cero, actualización sin perder partidas, controles, audio, guardado y salida.
3. Registra firmware, modelo, resolución, versión y hash del juego de prueba.
4. Ejecuta la verificación local y revisa `git diff --cached --stat` y `git ls-files` antes del commit.
5. Etiqueta una versión por port, por ejemplo `crossyroad-v0.1.0`, solo cuando el contenido publicado y sus limitaciones estén claros.

El ZIP creado por `tools/build-crossyroad-local.sh` es **privado**. No se adjunta a una release pública mientras dependa de componentes excluidos y no tenga licencias y pruebas suficientes.

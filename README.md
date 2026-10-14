# RG35XX-H Ports

Código y recetas de empaquetado para ports de juegos en consolas Linux. Cada juego vive en `ports/<nombre>/` y se versiona por separado dentro de este repositorio.

| Port | Estado | Equipo probado |
| --- | --- | --- |
| [Crossy Road](ports/crossyroad/README.md) | Jugable, fuente pública; paquete local privado | RG35XX-H, KNULLI, 640×480 |
| [Threes / fork de Triples](ports/threes-knulli/README.md) | Fork 0.6 funcional; código y herramientas de compilación | RG35XX-H, KNULLI |

Este repositorio **no contiene juegos ni paquetes listos para distribuir**. No incluye APK, recursos extraídos de juegos, ROMs, partidas, controladores de la consola ni ejecutables compilados. El código de Crossy Road está ligado al APK 4.5.0 indicado en su documentación y aún depende de un loader de investigación que no se puede reconstruir íntegramente desde esta fuente. Por ello, el port no es todavía una propuesta para el catálogo oficial de PortMaster.

## Organización

- `ports/crossyroad/portmaster/`: launcher, instalador y metadatos de PortMaster, sin componentes privados.
- `ports/crossyroad/source/`: puente JNI específico del juego y notas técnicas.
- `ports/threes-knulli/`: fuente de Triples adaptada a SDL2, compilación ARM64 y un lanzador con TATE seleccionable desde el menú.
- `tools/`: verificación de publicación y empaquetado **local** con componentes que el usuario proporciona aparte.
- `docs/WORKFLOW.md`: ramas, commits y preparación de entregas.

Para montar un paquete privado en tu propia máquina, sigue [la guía de Crossy Road](ports/crossyroad/README.md). El paquete generado queda ignorado por Git.

## Atribución y alcance

El código de Crossy Road conserva Apache-2.0 y los avisos de sus autores. Triples conserva su licencia Unlicense y sus fuentes tipográficas tienen licencia OFL. Consulta [ATTRIBUTION.md](ATTRIBUTION.md), [LICENSE](LICENSE) y [NOTICE](NOTICE), además de las licencias de cada port. Crossy Road y sus recursos pertenecen a Hipster Whale; este proyecto no está afiliado con ellos ni con PortMaster.

## Contribuciones

Usa una rama por port o corrección, ejecuta `sh tools/check-public-tree.sh` y sigue [CONTRIBUTING.md](CONTRIBUTING.md). Nunca añadas un APK, save, asset extraído o binario del sistema al historial de Git, ni siquiera temporalmente.

El fork de Triples mantiene su licencia Unlicense en `ports/threes-knulli/UNLICENSE`. La versión pública dibuja fichas propias mediante código; los gráficos extraídos de Threes usados en la instalación privada quedan ignorados por Git. Consulta `ports/threes-knulli/ASSETS-LICENSE.md` y `CREDITS.md`.

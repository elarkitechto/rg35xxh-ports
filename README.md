# Elarkitechto Ports

Código y recetas de empaquetado para ports de juegos en consolas Linux. Cada juego vive en `ports/<nombre>/` y se versiona por separado dentro de este repositorio.

| Port | Estado | Equipo probado |
| --- | --- | --- |
| [Crossy Road](ports/crossyroad/README.md) | Jugable, fuente pública; paquete local privado | RG35XX-H, KNULLI, 640×480 |

Este repositorio **no contiene juegos ni paquetes listos para distribuir**. No incluye APK, recursos extraídos, ROMs, partidas, controladores de la consola ni ejecutables compilados. El código de Crossy Road está ligado al APK 4.5.0 indicado en su documentación y aún depende de un loader de investigación que no se puede reconstruir íntegramente desde esta fuente. Por ello, el port no es todavía una propuesta para el catálogo oficial de PortMaster.

## Organización

- `ports/crossyroad/portmaster/`: launcher, instalador y metadatos de PortMaster, sin componentes privados.
- `ports/crossyroad/source/`: puente JNI específico del juego y notas técnicas.
- `tools/`: verificación de publicación y empaquetado **local** con componentes que el usuario proporciona aparte.
- `docs/WORKFLOW.md`: ramas, commits y preparación de entregas.

Para montar un paquete privado en tu propia máquina, sigue [la guía de Crossy Road](ports/crossyroad/README.md). El paquete generado queda ignorado por Git.

## Atribución y alcance

El código derivado conserva la licencia Apache-2.0 y los avisos de sus autores. Consulta [ATTRIBUTION.md](ATTRIBUTION.md), [LICENSE](LICENSE) y [NOTICE](NOTICE). Crossy Road y sus recursos pertenecen a Hipster Whale; este proyecto no está afiliado con ellos ni con PortMaster.

## Contribuciones

Usa una rama por port o corrección, ejecuta `sh tools/check-public-tree.sh` y sigue [CONTRIBUTING.md](CONTRIBUTING.md). Nunca añadas un APK, save, asset extraído o binario del sistema al historial de Git, ni siquiera temporalmente.

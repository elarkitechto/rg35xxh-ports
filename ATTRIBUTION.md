# Atribuciones y procedencia

## Alto's Adventure

Alto's Adventure and its game assets belong to Snowman. The user supplies the Android APK; this repository does not distribute it or extracted game files. The Android compatibility framework retains its Apache-2.0 license and upstream notices in [ports/altosadventure/LICENSE](ports/altosadventure/LICENSE) and [ports/altosadventure/NOTICE](ports/altosadventure/NOTICE). [elarkitechto](https://github.com/elarkitechto) adapted and packaged this port. It is independent of Snowman and PortMaster.

## Crossy Road

| Componente | Autor / titular | Procedencia y condiciones |
| --- | --- | --- |
| Crossy Road, personajes, nombres, imágenes y recursos del juego | Hipster Whale | [Sitio oficial](https://www.crossyroad.com/). El APK y sus recursos no se incluyen aquí. |
| Framework Android ARM64 `so-loader` y shims en los que se basó el trabajo de Crossy Road | `nextos_ports_android`, crédito conservado a felc18-blip / [initdream](https://github.com/initdream/nextos_ports_android) | Apache-2.0; se conserva el aviso original en `NOTICE`. El puente `jni_override.c` contiene adaptaciones específicas para este juego. |
| Bases del framework de NextOS | mtojek, proyectos [syberia_arm64](https://github.com/mtojek/syberia_arm64) y [lswtcs_arm64](https://github.com/mtojek/lswtcs_arm64) | Crédito y licencia Apache-2.0 indicados por el `NOTICE` de NextOS. |
| Formato y herramientas de empaquetado | [PortMaster](https://github.com/PortsMaster/PortMaster-New) | Se siguió su estructura pública; PortMaster no ha aceptado ni avalado este port. |
| Adaptación, controles, guardado, instalador y documentación de Crossy Road | [elarkitechto](https://github.com/elarkitechto) | Cambios de este repositorio bajo Apache-2.0 en la medida en que son originales; se preservan los avisos anteriores. |

## Componentes que se mantienen fuera del repositorio

- APK 4.5.0 y bibliotecas `libunity.so`, `libil2cpp.so` y `libmain.so`: software de Hipster Whale/Unity aportado por cada usuario.
- `libMali.so`: controlador extraído de la instalación de KNULLI del dispositivo de prueba; no se redistribuye aquí.
- Loader precompilado de investigación y su parche, `libOpenSLES.so` y la biblioteca JNI compilada: el repositorio conserva fuente y hashes para reconstrucción local, pero no publica esos binarios. La procedencia y reproducibilidad del loader requieren más trabajo antes de una distribución pública.
- Arte de portada, capturas, partidas y registros personales: no se publican.

La licencia del código del repositorio no concede derechos sobre el juego, el controlador Mali ni otros binarios excluidos. Consulta también [el estado técnico del port](ports/crossyroad/README.md).

## Triples / Threes

El fork `ports/threes-knulli/` parte de [Triples de Christopher Wellons](https://github.com/skeeto/triples), commit `83e8286d15a1d3631d110dd47df88fd94d28c85a`, bajo Unlicense. Las adaptaciones para RG35XX-H son de elarkitechto. Las fuentes Inter y Jua se conservan con licencias OFL en `ports/threes-knulli/licenses/`. Los gráficos originales de Threes no están incluidos en el historial público; sus autores figuran en `ports/threes-knulli/CREDITS.md`.

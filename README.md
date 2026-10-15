# RG35XX-H Ports

Ports para **RG35XX-H con KNULLI**. Instala PortMaster y consulta la guía del juego:

| Juego | Archivo que debes aportar | Guía |
| --- | --- | --- |
| Alto’s Adventure | APK Android 1.8.8 | [Instalación y controles](ports/altosadventure/README.md) |
| Crossy Road | APK Android 4.5.0 ARM64 | [Instalación y controles](ports/crossyroad/README.md) |
| Threes! | XAPK Free 3.1 (24576), con split ARM64 | [Instalación y controles](ports/threes/README.md) |


**Triples** es un [demake independiente en desarrollo](ports/triples/README.md). No está listo para PortMaster.

Los tres ports originales tienen [paquetes candidatos para PortMaster](docs/PORTMASTER.md).

## Instalación

1. Consigue el paquete del port. Este repositorio contiene el código y los scripts para [generar los paquetes](docs/BUILD.md); los ZIP no están incluidos en el árbol de archivos.
2. Extrae el ZIP en la carpeta **Ports** de la consola.
3. Copia tu APK o XAPK en la carpeta indicada por la guía del juego.
4. Abre el juego desde **Ports**. La primera ejecución prepara los archivos automáticamente.

No necesitas ejecutar instaladores de subcarpetas ni compilar en la consola.
Usa la versión exacta indicada: las demás no son intercambiables.

Probados en RG35XX-H/KNULLI. Son ports comunitarios y todavía no forman parte del catálogo oficial de PortMaster. El juego original y sus recursos no se distribuyen aquí.

Para reportar un problema, indica juego, consola, firmware y lo que ocurrió en [Issues](https://github.com/elarkitechto/rg35xxh-ports/issues). Adjunta el log del port sin incluir archivos del juego ni datos personales.

[Compilar](docs/BUILD.md) · [Contribuir](CONTRIBUTING.md) · [Créditos y licencias](ATTRIBUTION.md)

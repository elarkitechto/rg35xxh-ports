# Threes!

Port comunitario para **RG35XX-H / KNULLI**, con mando y guardado local.
Requiere PortMaster y tu archivo original de **Threes! Free 3.1 (24576), con base y split ARM64**.

## Instalar

1. Extrae el ZIP del port en **Ports**.
2. Copia `threes-free-3-1-24576.xapk` dentro de `ports/threes/`.
3. Abre **Threes!** desde Ports. La primera ejecución prepara todo automáticamente.

No ejecutes archivos de instalación por separado. Si actualizas, conserva la
carpeta de partidas y sustituye sólo los archivos del paquete.

## Controles

| Botón | Acción |
| --- | --- |
| Cruceta / ambos sticks | Mover fichas; navegar en menús |
| A / B en menús | Aceptar / atrás |
| L1 / R1 en menús | Cambiar pantalla lateral |
| Izquierda / derecha en menús | Cambiar pantalla lateral |
| Arriba / abajo en menús | Seleccionar opción |
| Start / Select durante la partida | Menú / estadísticas |
| L2 + R2 | Alternar TATE y vista horizontal |
| F + Start o Select + Start | Guardar y salir |

Inicia en TATE: gira la consola 90° hacia la derecha. En TATE, los cuatro botones
frontales también mueven fichas durante el juego; en los menús A/B siguen siendo
aceptar/atrás. La vista horizontal conserva la proporción del tablero y extiende
el fondo. La firma de puntuación usa el nombre predeterminado.

El tema nocturno conserva las fichas nocturnas originales. Threes! y el demake
Triples son juegos y guardados separados.

La versión Free mantiene sus restricciones originales de partidas. Los vídeos,
servicios online y compras no están disponibles; este port no los desbloquea.

Partidas: `/userdata/system/threes-android/saves/`.

## Si no arranca

Comprueba el nombre y la versión del archivo, y consulta `threes/install.log`
o `threes/log.txt`. No se ha verificado compatibilidad con otras consolas o
firmwares. No es una publicación oficial de PortMaster.

<details>
<summary>Identificar el archivo compatible (SHA-256)</summary>

`99dcba5f8dbcbc4294507c6b30d9f0719ae4127228186e9a471ecdb45a77c880`

</details>

Más información: https://github.com/elarkitechto/rg35xxh-ports

Gracias a Sirvo por crear el juego original.

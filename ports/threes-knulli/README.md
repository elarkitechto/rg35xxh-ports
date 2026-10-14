# Triples para RG35XX H / KNULLI Scarab

El código de esta carpeta está en desarrollo. La instalación privada y funcional de la consola se conserva por separado, fuera de este repositorio; **no sustituyas esa instalación** con una compilación de esta rama hasta verificarla en la consola.

Fork de Triples para Linux ARM64, basado en [Triples](https://github.com/skeeto/triples), commit
`83e8286d15a1d3631d110dd47df88fd94d28c85a`, con los gráficos de la demo web
[Threes](http://play.threesgame.com/). El motor se adaptó de SDL3 a SDL2 para
utilizar la biblioteca y el controlador de vídeo de KNULLI.

## Dos variantes de recursos

La versión pública usa las fichas originales generadas por el código de Triples y no incluye gráficos extraídos de Threes. Las fuentes Inter y Jua sí se incluyen con sus licencias OFL en `licenses/`.

La variante privada puede usar los gráficos originales que ya tienes localmente en `assets/skin/`. Esos archivos están excluidos de Git. Para copiarlos a una compilación ARM64 local usa `--private-skin` en `tools/build_arm.py`; para empaquetarlos localmente usa `--private-skin` en `tools/package.py`. **No publiques ese ZIP privado.**

## Cambios en desarrollo para 0.5

- Pantalla de carga al abrir.
- Tres páginas de reglas la primera vez que se juega, accesibles después en **Menú → Cómo jugar**.
- Segundo lanzador TATE para girar físicamente la consola 90° en sentido horario; comparte partida con Horizontal.

Estas funciones tienen pruebas de escritorio. TATE aún necesita confirmación con el controlador de vídeo de la RG35XX-H.

## Instalar en la consola

1. Instala PortMaster desde KNULLI si aún no lo tienes.
2. Extrae el ZIP generado localmente en `/userdata/roms/ports/`.
   Deben quedar `Threes.sh`, `Threes TATE.sh` y la carpeta `threes-knulli` juntos.
   Desde Windows puedes usar el recurso de red de KNULLI, carpeta `share/roms/ports`.
3. Actualiza la lista de juegos de KNULLI y abre **Ports → Threes** o **Threes TATE**.

El segundo lanzador gira la imagen para jugar con la consola rotada 90° en sentido horario. La cruceta se adapta a esa orientación y ambos lanzadores comparten la misma partida. La variante TATE requiere que el controlador SDL2 de KNULLI admita texturas de destino; está pendiente de prueba en el equipo.

El ZIP conserva permisos de ejecución. Si tu método de copia los pierde,
restáuralos en `Threes.sh` y `threes-knulli/threes` con `chmod +x`.
No sustituyas la biblioteca SDL2 del sistema: el ejecutable usa la de KNULLI.

Para actualizar desde 0.1 o 0.2, cierra el juego y extrae el ZIP en esa misma carpeta,
reemplazando los archivos existentes. La actualización no incluye archivos en
`saves`, por lo que conserva tu partida y récords.

## Cambios en 0.4

- Fondo gris carbón #292D33, sin modificar fichas, tablero ni panel de siguiente.
- Textos exteriores claros para mantener el contraste.
- Tema Oscuro/Claro seleccionable en el menú y guardado con el resto de preferencias.
- Oscuro es el valor inicial, también al actualizar ajustes anteriores sin tema.
- El fondo se dibuja con un color plano: no requiere PNG adicionales.

## Cambios en 0.3.1

- El crédito de elarkitechto identifica el proyecto como fork de Triples en ambos idiomas.

## Cambios en 0.3

- Menú de pausa con Start; A elige, B vuelve y la cruceta navega.
- Volumen entre 0 y 100 por pasos de 10. A sobre volumen silencia o reactiva.
- Español e inglés para los textos del juego, menú y créditos.
- Volumen, idioma y tema se guardan en saves/settings.dat.
- Créditos originales, Triples / Christopher Wellons (skeeto) y elarkitechto.

## Cambios en 0.2

- Marco del tablero con el recurso original `board.png`: conserva esquinas,
  relieve superior y tonos aguamarina al adaptarse a la pantalla horizontal.
- Fondo original `preview_bg.png` detrás de la ficha de **Siguiente**.
- El tamaño y las posiciones de las fichas del tablero se mantienen.

## Controles

| Control | Acción |
| --- | --- |
| Cruceta | Deslizar una casilla |
| Y | Nueva partida |
| Start | Abrir o cerrar el menú |
| A | Elegir opción del menú |
| B | Volver |
| Select | Salir |
| PC: flechas / WASD | Deslizar |
| PC: R / Esc | Nueva partida / salir; Esc vuelve si hay un menú |
| PC: Tab / Enter / Retroceso | Menú / elegir / volver |

Y reinicia inmediatamente fuera del menú. Cada movimiento guarda la partida; los archivos
quedan en `threes-knulli/saves`. El lanzador usa el mapeo de PortMaster y,
cuando está disponible, gptokeyb. Desactiva entonces la entrada nativa para
evitar procesar dos veces una pulsación.

## Qué está hecho y qué falta validar

- Fichas, personajes, casillas vacías y vista previa extraídos del atlas original.
- Tablero 4 × 4 en pantalla horizontal de 640 × 480; puntuación a la derecha.
- Movimiento de botones interpolado durante 140 ms, aparición de fichas,
  rebote de combinaciones y giro de reinicio heredados de Triples.
- Sonido sintetizado de Triples adaptado a mezcla simultánea con SDL2.
- Binario Linux AArch64 para Cortex-A53, sin dependencia de libstdc++.
- 33 pruebas pasan: 17 326 comprobaciones. Prueba de escritorio de 90 fotogramas:
  inicio, tres movimientos, captura visual y salida con código 0.

**El usuario confirmó que la versión 0.1 corre perfectamente en su RG35XX H
con KNULLI Scarab.** La actualización 0.4 compila para el mismo destino
y se verificó en escritorio: pausa sin cambios en la partida, ajustes guardados,
créditos, reinicio con idioma conservado y tres movimientos al volver al juego; falta
confirmar esta nueva versión en la consola. No se ha medido allí la tasa de fotogramas.
El audio, los menús y las animaciones de los rostros no son una reproducción
completa del original. Las fichas de valores muy altos se ajustan al tamaño de
la celda; el rango adicional sin recurso original conserva el dibujo de Triples.

Si vuelve al menú o muestra una pantalla negra, recupera
`threes-knulli/log.txt` para diagnosticarlo. Indica también si se oye sonido
y si responde Select. No es necesario cambiar de firmware.

## Compilar

Linux: CMake 3.24+, compilador C++20 y SDL2 2.0.14 o posterior.

```sh
cmake -S . -B build -DTHREES_SYSTEM_SDL2=ON
cmake --build build -j
ctest --test-dir build --output-on-failure
```

Windows: MinGW-w64 y CMake; SDL2 se descarga y se compila estáticamente.

```sh
cmake -S . -B build -G "MinGW Makefiles" -DTHREES_SYSTEM_SDL2=OFF
cmake --build build -j
```

Compilación cruzada reproducible desde Windows o Linux, con Zig 0.14.1 y Python:

```sh
python tools/get_arm_sdk.py build-sdk
python tools/build_arm.py --zig /ruta/a/zig --sdk build-sdk --out build-arm
```

El SDL2 de Debian se usa sólo como referencia de enlace; no se distribuye en
el paquete. El programa requiere `libSDL2-2.0.so.0` del firmware y funciones
de glibc hasta 2.27. La versión pública usa arte dibujado por el motor. Los scripts y recursos
empleados para preparar la variante personal con gráficos originales no forman
parte de este repositorio.

## Créditos y uso de los recursos

Triples y sus modificaciones de código mantienen la Unlicense.
Los gráficos originales conservan sus derechos propios: **la Unlicense no
los cubre**. Este paquete es un prototipo personal; no se ha establecido
permiso para publicar o redistribuir esos gráficos. Véase `ASSETS-LICENSE.md`.
`README.upstream.md` conserva la documentación del proyecto original; algunas
rutas históricas descritas allí no se incluyen en este fork para KNULLI.

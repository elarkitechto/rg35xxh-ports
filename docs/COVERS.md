# Portadas de Ports en KNULLI

La colección de esta consola usa **box art 2D vertical**. La resolución del panel (640×480) y el formato horizontal de la captura de PortMaster no definen la portada del catálogo.

## Parámetros

- Portada: arte frontal vertical del juego, con título legible y composición de caja. Preferir relación 2:3 (por ejemplo 600×900). Conservar la proporción original de una portada adecuada: no estirarla para forzar esas medidas. El historial también aceptó Threes! a 528×704.
- Captura: gameplay horizontal 4:3, mínimo 640×480, en `screenshot.png`. Es un archivo distinto de `cover.png`; no reutilizar una captura como portada del catálogo.
- Priorizar arte del juego concreto. No sustituir Alto’s Adventure por Alto’s Odyssey o The Alto Collection. Si no hay portada adecuada, preparar una composición vertical a partir de sus propios recursos y verificarla visualmente.
- Conservar título, personajes y elementos principales completos; evitar recortes o bandas añadidas sólo para alterar la relación de aspecto. Usar PNG o JPEG según los metadatos; el empaquetador de Alto recibe PNG.
- PortMaster: `cover.png` y `screenshot.png` deben mantenerse separados; `gameinfo.xml` apunta a `./<port>/cover.png` e incluye también `<thumbnail>` hacia la misma portada.
- Consola: guardar la portada en `roms/ports/images/boxart/<port>.png`; tanto `<image>` como `<thumbnail>` de `gamelist.xml` apuntan a ella.
- Respaldar metadatos, conservar lanzadores, partidas y estadísticas, y refrescar EmulationStation cuando no haya una partida activa.

## Verificación antes de cerrar un port

1. Abrir y revisar la portada, no limitarse a comprobar dimensiones.
2. Confirmar que es vertical, tiene el nombre correcto y no es una captura de gameplay.
3. Verificar las rutas de imagen y miniatura en la consola y en el paquete.
4. Confirmar que la portada instalada y las copias del ZIP coinciden por SHA-256.
5. Documentar fuente, dimensiones y hash del arte; mantener archivos privados fuera de Git según CONTRIBUTING.md.

El empaquetador de Alto ejecuta `tools/validate-port-media.py`: rechaza portada horizontal/cuadrada, captura que no sea 4:3 de al menos 640×480 y el mismo archivo usado para ambos fines. Esto no sustituye la revisión visual.

## Procedencia del criterio

Recuperado del chat **Investigate Knulli port loading and**, del mismo proyecto: revisión de ScreenScraper (box art 2D), homologación de Marvel Cosmic Invasion y Streets of Rage 4 (600×900), Threes! (528×704), portada de 2048 Plus (2:3) y cierre de Crossy Road (portada vertical preservada al empaquetar). Los 640×480 mencionados posteriormente para Triples se referían a su presentación dentro del juego; no sustituyen el criterio de caja vertical del catálogo.

## Alto’s Adventure

Portada seleccionada: [LaunchBox, Fanart – Box – Front para Android](https://gamesdb.launchbox-app.com/games/images/101151-altos-adventure). Archivo original [PNG](https://images.launchbox-app.com/3583a3ee-f75a-4eeb-8563-09b0695d8b7f.png), 654×1024, conservado sin deformación. La captura real de la consola permanece a 640×480. El arte no se incluye en el historial público de Git.

SHA-256 de la portada de Alto: `68c67d8974fadc30b8642c1cae8a48b3296d46cd6e9d3154092fc20f4aaf4b45`.

# Portadas de Ports en KNULLI

Las portadas del catálogo usan **box art 2D vertical**. La resolución de pantalla 640×480 y la captura horizontal de PortMaster no determinan el formato de la portada.

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

## Alto’s Adventure

Portada seleccionada: [LaunchBox, Fanart – Box – Front para Android](https://gamesdb.launchbox-app.com/games/images/101151-altos-adventure). Archivo original [PNG](https://images.launchbox-app.com/3583a3ee-f75a-4eeb-8563-09b0695d8b7f.png), 654×1024, conservado sin deformación. La captura real de la consola permanece a 640×480. El arte no se incluye en el historial público de Git.

SHA-256 de la portada de Alto: `68c67d8974fadc30b8642c1cae8a48b3296d46cd6e9d3154092fc20f4aaf4b45`.


## Crossy Road

El candidato reutiliza la portada vertical ya instalada y aprobada en esta colección:1080×1683, sin deformación; se inspeccionó visualmente. La procedencia original debe confirmarse antes de redistribuir el arte a terceros. Se conserva una captura de gameplay640×480 separada. Portada/thumbnail del paquete: `./crossyroad/cover.png`. El empaquetador también valida ambas imágenes.

SHA-256 de la portada: `227c1360c3893d164fc979f50aa0996368872c782cad3534475b1a5c01f61ed7`. Arte privado excluido del historial público de Git.

## Threes! and Triples

Threes! (original Android port) uses the formerly demake-assigned cover from
console `images/boxart/threes.jpg`, 300×450, converted to PNG without resizing.
Upstream provenance of that pre-existing image was not recorded. Triples uses
a built-in ImageGen title edit of the same composition, 1024×1536, reading
TRIPLES. Both are vertical 2:3 and were visually inspected. Private assets and
the exact generation prompt are in `.local/threes-media/ARTWORK.md`; hashes
are in `.local/threes-media/artwork.sha256`. Console image and thumbnail paths:
`./images/boxart/threes.png` and `./images/boxart/triples.png`, respectively.
Runtime/save folders remain threesandroid and threes-knulli.

# Contribuir

1. Abre una rama por cambio: `port/crossyroad-tema`, `fix/crossyroad-tema` o `docs/tema`.
2. Haz commits pequeños y descriptivos: `feat(crossyroad): ...`, `fix(crossyroad): ...`, `docs: ...`, `chore: ...`.
3. Ejecuta `sh tools/check-public-tree.sh` y valida el launcher e instalador con `bash -n` y `sh -n` antes de abrir una solicitud de cambios.
4. Documenta el equipo, firmware, versión exacta del juego y pruebas realizadas. Una prueba en RG35XX-H/KNULLI no demuestra compatibilidad con otras consolas.
5. Conserva los avisos de licencia y atribuye cualquier código ajeno. Añade solo archivos cuya publicación esté permitida.

No subas APK, ROM, datos extraídos, saves, controladores, binarios compilados, logs de usuario ni archivos de `.local/`. Una vez que Git guarda un archivo en un commit, añadirlo a `.gitignore` no lo borra del historial.

La [guía de PortMaster](https://github.com/PortsMaster/PortMaster-New) exige una estructura, metadatos, licencias y pruebas más amplias para una propuesta oficial. Este repositorio no afirma cumplir todavía todos esos requisitos.

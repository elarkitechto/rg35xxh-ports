# Candidatos de PortMaster

| Port | Carpeta y paquete | Estado |
| --- | --- | --- |
| Alto’s Adventure | `altosadventure` / `altosadventure.zip` | Candidato de pruebas |
| Crossy Road | `crossyroad` / `crossyroad.zip` | Candidato de pruebas |
| Threes! original | `threes` / `threes.zip` | Candidato de pruebas |
| Triples, demake | `ports/triples` | En desarrollo; excluido |

Genera los paquetes con las instrucciones de [compilación](BUILD.md).
Cada comando compila y empaqueta el runtime completo. El usuario aporta el APK
compatible y abre el launcher; la preparación de datos ocurre automáticamente.
No hay un segundo instalador que ejecutar en la consola.

Para preparar el árbol de entrega, desde la raíz del repositorio:

```sh
python3 tools/prepare-portmaster.py altosadventure
python3 tools/prepare-portmaster.py crossyroad
python3 tools/prepare-portmaster.py threes
```

La salida privada es `.local/portmaster/port/<nombre>/`. Incluye metadatos,
README, captura, portada, launcher, runtime y licencias. El preparador valida los
archivos obligatorios y rechaza APK/XAPK y partidas. No sobrescribe salidas
anteriores: usa `--output` para preparar otra revisión.

Estos candidatos siguen la [estructura de PortMaster](https://portmaster.games/packaging.html).
Antes de solicitar su inclusión, hay que ejecutar también los checks oficiales
del repositorio PortMaster-New sobre ese árbol y resolver su revisión de
compatibilidad y licencias. Sólo se ha probado RG35XX-H con KNULLI; el runtime
usa las bibliotecas gráficas del dispositivo. No se afirma compatibilidad con
otros equipos ni aceptación oficial. No se redistribuyen los juegos originales.

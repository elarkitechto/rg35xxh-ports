# Crossy Road

Port comunitario para **RG35XX-H / KNULLI**, con mando y guardado local.
Requiere PortMaster y tu archivo original de **Crossy Road 4.5.0 ARM64**.

## Instalar

1. Extrae el ZIP del port en **Ports**.
2. Copia `crossy-road-4-5-0.apk` dentro de `ports/crossyroad/`.
3. Abre **Crossy Road** desde Ports. La primera ejecución prepara todo automáticamente.

No ejecutes archivos de instalación por separado. Si actualizas, conserva la
carpeta de partidas y sustituye sólo los archivos del paquete.

## Controles

| Botón | Acción |
| --- | --- |
| Cruceta / stick izquierdo | Mover / navegar |
| B | Aceptar / continuar |
| Y | Atrás |
| Start | Pausar / continuar |
| F + Start | Guardar y salir |

Los servicios online y anuncios no están disponibles. El progreso se guarda
localmente; se reutilizan las partidas existentes de `/userdata/system/cr/`.

## Si no arranca

Comprueba el nombre y la versión del archivo, y consulta `crossyroad/install.log`
o `crossyroad/log.txt`. No se ha verificado compatibilidad con otras consolas o
firmwares. No es una publicación oficial de PortMaster.

<details>
<summary>Identificar el archivo compatible (SHA-256)</summary>

`1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb`

</details>

Más información: https://github.com/elarkitechto/rg35xxh-ports

Gracias a Hipster Whale por crear el juego original.

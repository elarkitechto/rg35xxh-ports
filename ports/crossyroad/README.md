# Crossy Road (KNULLI RG35XX-H)

Port experimental basado en el APK Android **4.5.0**. En el equipo probado funcionan cruceta y stick izquierdo, B para aceptar, Y para volver, Start para pausar y F+Start para guardar y salir. Las partidas se crean en la consola; este repositorio no contiene ninguna.

El paquete privado para RG35XX-H añade vibración breve al salto, una más fuerte y prolongada a la muerte, tres pulsos descendentes al entrar un tren y un pulso por cada sacudida de cámara. Los hooks comprueban los opcodes del APK 4.5.0 antes de instalarse.

## APK requerido

El usuario debe aportar legalmente `crossy-road-4-5-0.apk`, SHA-256:

`1ad8bed1353b36a953c020c1562a519adceda02e4b4e98afa8eaf0db87643afb`

El instalador comprueba el hash, extrae los recursos y las bibliotecas ARM64 del APK en el dispositivo y preserva las partidas existentes. También desactiva Unity Analytics para evitar escrituras continuas en la SD. Ningún recurso del juego se publica aquí.

## Estado de reproducción

Los scripts y el puente JNI están publicados, pero el loader base fijado durante la investigación y las bibliotecas de dispositivo aún no tienen una cadena de construcción pública completa. Para reproducir **localmente** el paquete probado se necesitan los componentes exactos descritos en `tools/build-crossyroad-local.sh`, colocados en `.local/components/`. El script verifica sus hashes. El paquete resultante permanece en `.local/releases/`, fuera de Git.

```sh
sh tools/build-crossyroad-local.sh
```

El archivo ZIP generado se puede instalar localmente por PortMaster. Tras instalar, se copia el APK al directorio `crossyroad` de Ports y se abre **Crossy Road una sola vez**: ese mismo launcher comprueba el APK, extrae sus recursos, instala o actualiza el runtime y arranca el juego. En una consola nueva crea su propio save; en una existente conserva el progreso. No se debe distribuir ese ZIP mientras incluya binarios o arte no autorizados.

Para reconstruir el puente JNI, usa `ports/crossyroad/source/build_native.sh`. El parche reproducible del loader está en `ports/crossyroad/source/patch_loader_sdl.py`; requiere el loader base exacto, aportado por separado. La [guía técnica](source/UNITY_ANDROID_PORTING.md) explica qué decisiones son específicas de este juego.

## Límites

Probado en RG35XX-H con KNULLI y pantalla 640×480. Falta reproducir el loader completo desde código fuente y comprobar otras consolas. Este árbol fuente no constituye todavía una entrega oficial de PortMaster.

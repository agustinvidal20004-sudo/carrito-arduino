# Verificación del proyecto

Fecha: 2 de noviembre de 2026.

## Compilación para Arduino Uno

Herramientas: Arduino CLI 1.5.1, Arduino AVR Boards 1.8.8.
FQBN: arduino:avr:uno.

Los cuatro programas compilan para el microcontrolador del Uno:

| Programa | Flash | RAM global |
|---|---:|---:|
| Prueba_Placa | 1992 bytes | 226 bytes |
| Prueba_Un_Sensor | 2182 bytes | 250 bytes |
| Prueba_Sensores | 2448 bytes | 316 bytes |
| Control_USB | 3476 bytes | 199 bytes |

El programa de control incluye un prototipo explícito para la función que
recibe el enum Movimiento; evita problemas con los prototipos automáticos
que Arduino genera al preprocesar el sketch.

## Qué se probó físicamente antes

- El usuario confirmó carga y parpadeo del LED del Uno.
- El usuario confirmó funcionamiento de la prueba de un sensor sobre blanco/negro.

## Qué falta probar

- Control_USB no se ha probado físicamente con el L298N y los motores.
- La alimentación del montaje final y el puente 5V-EN requieren revisión.
- Se comprobó el script Linux y se usó para compilar los cuatro sketches.
- No se ejecutó carrito.cmd en un PC Windows durante esta preparación.
  Se debe confirmar instalación y carga en el Windows del equipo.
- No hay soporte Bluetooth en esta versión.

Una compilación correcta comprueba que el programa se puede generar para
el Uno; no demuestra que el cableado, la potencia o el sentido de giro sean correctos.

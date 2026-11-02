# Carrito con Arduino Uno: control por USB

Programa: Control_USB.ino. Comunicación serie: 9600 baudios.
Esta versión no utiliza sensores ni PID. El L298N controla los dos motores.
No se ha cargado a la placa ni probado con motores.

## Órdenes

- W: avanzar.
- S: retroceder.
- A: girar a izquierda, deteniendo la rueda izquierda.
- D: girar a derecha, deteniendo la rueda derecha.
- X: deshabilitar ambos motores; el carrito puede seguir por inercia.
- + / -: subir o bajar el PWM en pasos de 20 (rango 0–255).
- ?: consultar el movimiento solicitado y el PWM.

Acepta minúsculas. Ignora espacios y finales de línea.
Una orden desconocida detiene los motores.

Cada orden W/S/A/D autoriza movimiento durante 1500 ms. Para continuar,
hay que repetir la orden antes de ese tiempo. Consultar estado o cambiar
PWM no prolonga el movimiento. Al vencer el plazo se necesita otra orden
de movimiento: no reanuda por sí solo. Esta es una parada por software,
no detecta todos los fallos del microcontrolador y no es un freno mecánico.

En un monitor serie que envía por líneas, escribir W y pulsar Enter.
Mantener pulsada W en una terminal no equivale necesariamente a enviar
órdenes continuamente. Para movimiento sostenido, un futuro controlador
del PC puede transmitir la orden cada 200 ms mientras la tecla esté activa.

## Pines propuestos (no conectar alimentación todavía)

| Arduino Uno | L298N | Función |
|---|---|---|
| D5 | ENA | PWM motor izquierdo |
| D7 | IN1 | Dirección izquierda |
| D8 | IN2 | Dirección izquierda |
| D6 | ENB | PWM motor derecho |
| D9 | IN3 | Dirección derecha |
| D10 | IN4 | Dirección derecha |
| GND | GND | Referencia común |

Canal A (OUT1/OUT2): motor izquierdo; canal B (OUT3/OUT4): derecho.
Izquierda/derecha se definen mirando hacia delante desde el carrito.
D2 y D3 quedan reservados para los sensores si se añaden en otra versión.

Para PWM, se retirarán los puentes ENA y ENB antes de conectar las salidas
del Arduino. El puente 5V-EN es diferente: no modificar ni conectar el borne
5V hasta revisar la foto del módulo y confirmar su alimentación.
Queda pendiente definir voltaje, límite de corriente y conexiones de la
fuente según los motores y el módulo reales. No alimentar motores desde
el pin 5V del Uno. Hacer las primeras pruebas con las ruedas levantadas.

## Organización del código

setup(): configura salidas, deshabilita motores y abre el puerto serie.
procesarOrden(): interpreta un carácter y solicita el movimiento.
actualizarMotores(): comprueba el tiempo sin órdenes y aplica el movimiento.

No usa delay(). Al cambiar de movimiento introduce al menos 100 ms con
los motores deshabilitados. Esa pausa no garantiza que las ruedas ya se
hayan detenido. El PWM inicial es 120: no representa una velocidad medida
ni reduce el voltaje de los pulsos de alimentación al valor nominal del motor.

Si una rueda gira al revés, se puede ajustar INVERTIR_IZQUIERDO o
INVERTIR_DERECHO a true después de identificar qué rueda es.

## Verificación pendiente en el Uno

Primero compilar para arduino:avr:uno. Después, con el cableado revisado:
comprobar arranque detenido, sentidos de giro, X, parada tras 1500 ms sin
órdenes y cambios de potencia. La sintaxis comprobada en PC no sustituye
la compilación para el Uno ni las pruebas físicas.

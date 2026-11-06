# Bluetooth y app Arduino Car (ONE DAY OF CODE)

Esta variante corresponde a las conexiones de señal del diagrama aportado
por el equipo, del video de El Profe Ricardo:
https://www.youtube.com/watch?v=k6zf60cpILw
No se está certificando todo el circuito de alimentación del dibujo.

## Diferencias con Control_USB

| Señal | Arduino Uno |
|---|---|
| TXD del módulo Bluetooth | D2, entrada RX del Arduino |
| RXD del módulo Bluetooth | Dejar desconectado para estas pruebas de solo recepción |
| IN1 del L298N | D5 |
| IN2 del L298N | D6 |
| IN3 del L298N | D9 |
| IN4 del L298N | D10 |

SoftwareSerial bluetooth(2, 3) usa RX=2 y TX=3 desde el punto de vista del
Arduino. D3 queda reservado. No conectar los sensores a D2/D3 en esta variante.
El programa solo recibe Bluetooth; los mensajes de diagnóstico salen por USB.

En esta variante, ENA y ENB del L298N deben tener sus puentes instalados.
El PWM se aplica a IN1/IN2 y a IN3/IN4. Control_USB usa otro cableado: no
intercambiar programas sin comprobar conexiones. Se asume canal A como
motor izquierdo y canal B como derecho; confirmar el montaje físico.

## Dos puntos eléctricos que no se deben copiar a ciegas del dibujo

- El Uno transmite con lógica de 5 V. RXD de muchos HC-05/HC-06 requiere
  nivel de 3,3 V aunque VCC de su placa adaptadora admita 5 V. No conectar
  D3 directamente a RXD salvo que la ficha de la placa confirme tolerancia.
  Como estas versiones no envían datos al módulo, dejar RXD desconectado
  evita necesitar esa conexión. Si luego se necesita transmisión, usar un
  adaptador de nivel o un divisor calculado para la placa real.
- Confirmar VCC del módulo en su ficha/serigrafía; no confundir la placa
  adaptadora regulada con el módulo desnudo. GND del módulo y del Uno deben
  estar unidos. La alimentación del L298N y su puente 5V-EN se revisan aparte.
  Cuatro AA alcalinas dan 6 V nominales: no garantizan los 7–12 V recomendados
  para VIN del Uno ni una entrada suficiente al regulador de 5 V del L298N.

Para la primera prueba: Uno por USB, motores sin alimentación, módulo con
su VCC adecuado y GND común. El programa de diagnóstico mantiene D5/6/9/10
bajos, pero no sustituye desconectar la alimentación de los motores.

## 1. Descargar y probar recepción (Windows)

Desde la carpeta clonada:

```powershell
git pull --ff-only
.\carrito.cmd puertos
.\carrito.cmd cargar Prueba_Bluetooth COM3
.\carrito.cmd monitor Prueba_Bluetooth COM3
```

Sustituir COM3 por el puerto del equipo. El monitor USB usa 9600 baudios.
El enlace UART del módulo también se configura a 9600 en el sketch; debe
coincidir con la configuración de datos del módulo. Si no coincide, ajustar
BAUD_BLUETOOTH y volver a cargar. No se cambia automáticamente la
configuración interna del HC-05/HC-06.

Emparejar y conectar la app al módulo siguiendo su procedimiento. Conceder
los permisos Bluetooth que Android solicite para esa conexión. Pulsar un
botón: el monitor debe mostrar bytes, por ejemplo `hex=0x46 ... caracter='F'`.
Comprobar por separado qué envía al pulsar, mantener y soltar. Si se ven
caracteres ilegibles, revisar baudrate, GND, cableado y módulo.

## 2. Configurar la app: modo de botones digitales

La ficha del desarrollador anuncia códigos configurables por botón:
https://play.google.com/store/apps/details?id=com.electro_tex.bluetoothcar
No se asumen códigos predeterminados ni que todas las versiones tengan
los mismos menús. Configurar y verificar usando Prueba_Bluetooth:

| Acción | Carácter ASCII (sin comillas) |
|---|---|
| Avanzar | F |
| Retroceder | B |
| Izquierda | L |
| Derecha | R |
| Detener / soltar el botón | S |
| Subir/bajar potencia, si se configura | + / - |

S significa PARAR en Control_Bluetooth; en Control_USB significaba
retroceder. Las dos variantes tienen protocolos distintos.
No activar joystick analógico, acelerómetro, diagonales o deslizadores
hasta conocer sus mensajes: sus formatos aún no están implementados.
Comando desconocido: parada. S y X son órdenes de parada.

El control conserva el límite de 1500 ms sin otra orden de movimiento.
Si la app no repite F/B/L/R mientras se mantiene un botón, el vehículo se
parará tras 1,5 s aunque se siga pulsando. No eliminar ese límite para
ocultar el problema; primero comprobar el comportamiento real de la app.
Ni +, -, ? ni saltos de línea renuevan el plazo de movimiento.

## 3. Probar control solo después del diagnóstico

Revisar alimentación y cableado y levantar las ruedas. Cerrar el monitor con
Ctrl+C, cargar y abrir de nuevo:

```powershell
.\carrito.cmd cargar Control_Bluetooth COM3
.\carrito.cmd monitor Control_Bluetooth COM3
```

Arranca detenido. El USB solo muestra diagnóstico. El movimiento se ordena
por Bluetooth. Probar F y S primero, después B/L/R. Comprobar parada al soltar
y al dejar de enviar órdenes. Cortar alimentación de motores si el sentido
no corresponde; invertir los cables del motor con todo apagado o ajustar
INVERTIR_IZQUIERDO/INVERTIR_DERECHO en el sketch.

Con ENA/ENB habilitados, poner ambas entradas de un canal a LOW produce
frenado eléctrico según el L298; no es una parada de emergencia certificada.
La pausa de 100 ms entre movimientos no garantiza que las ruedas ya estén
detenidas. El PWM inicial es 120/255 y no es regulación de velocidad por PID.

## Linux

Las mismas acciones usan el script Bash y el puerto detectado:

```bash
bash carrito.sh cargar Prueba_Bluetooth /dev/ttyUSB0
bash carrito.sh monitor Prueba_Bluetooth /dev/ttyUSB0
```

## Verificación

Ambas nuevas variantes se compilaron localmente para arduino:avr:uno con
Arduino CLI 1.5.1 y AVR Boards 1.8.8. No se han probado físicamente con el
módulo, la aplicación y los motores del equipo. La prueba de recepción
permite verificar primero el protocolo real de la aplicación.

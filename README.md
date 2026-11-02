# Carrito Arduino Uno

Código del equipo para Arduino Uno + controlador de motores L298N.
Control actual: órdenes por USB desde terminal. Bluetooth aún no implementado.

## Qué instalar en Windows

1. **Git for Windows**: https://git-scm.com/install/windows
2. Windows 10/11 de 64 bits, con PowerShell y curl (incluidos normalmente).
3. Internet durante la instalación inicial. Después se puede compilar y cargar sin conexión.

No hacen falta Arduino IDE, Python ni librerías externas. El comando `instalar`
descarga Arduino CLI **1.5.1** del servidor oficial de Arduino y el paquete
**Arduino AVR Boards 1.8.8**. Se guardan dentro de esta carpeta y no se añaden al PATH.

Si `winget` está disponible, se puede instalar Git desde PowerShell o CMD:

```powershell
winget install --id Git.Git -e --source winget
```

Cerrar y volver a abrir la terminal después de instalar Git.
Si no hay winget, usar el instalador oficial indicado arriba.

## Primera descarga (Windows: PowerShell o CMD)

```powershell
git clone https://github.com/agustinvidal20004-sudo/carrito-arduino.git
cd carrito-arduino
.\carrito.cmd instalar
```

El repositorio es público: no se requiere cuenta para clonar o descargar.
Eso no concede permiso para modificar el repositorio remoto.

## Primera prueba: LED del Uno

Dejar motores y L298N sin alimentación. Conectar el Arduino al PC mediante
el cable USB de datos.

```powershell
.\carrito.cmd puertos
```

Buscar su puerto (por ejemplo COM3). Una placa compatible puede aparecer como
`Unknown`: eso no impide seleccionarla como Arduino Uno, si ese es el hardware.
Reemplazar **COM3** por el puerto que aparezca en el equipo:

```powershell
.\carrito.cmd cargar Prueba_Placa COM3
```

El comando primero compila; si falla, no intenta cargar.
El LED L debe parpadear cada medio segundo. No hace falta cableado externo.

## Qué programa elegir

Solo un programa queda cargado a la vez: la nueva carga reemplaza al anterior.

| Programa | Función | Conexiones |
|---|---|---|
| `Prueba_Placa` | Parpadeo del LED integrado | Solo USB |
| `Prueba_Un_Sensor` | Lectura 0/1 y LED L según la entrada | OUT en D2, GND común, VCC según especificación del sensor |
| `Prueba_Sensores` | Lecturas de dos sensores | OUT izquierdo D2, derecho D3, alimentación apropiada y GND común |
| `Control_USB` | Control de dos motores con L298N | Revisar `docs/CONTROL_Y_CONEXIONES.md` antes de alimentar |

Para comprobar código sin tener placa conectada:

```powershell
.\carrito.cmd compilar Control_USB
```

Para probar **un sensor** después de revisar sus conexiones:

```powershell
.\carrito.cmd cargar Prueba_Un_Sensor COM3
.\carrito.cmd monitor Prueba_Un_Sensor COM3
```

Poner blanco y negro a la misma distancia. La lectura debe cambiar de forma
estable. No asumir qué color corresponde a 0 o 1 antes de medir.
Ctrl+C termina el monitor; desconectar el USB apaga el Uno si es su única alimentación.

## Control del carrito por USB

**La alimentación de motores y el puente 5V-EN todavía requieren confirmar el
montaje real. Compilar correctamente no valida el cableado.** Empezar con las
ruedas levantadas. Consultar las conexiones y limitaciones en
[Control y conexiones](docs/CONTROL_Y_CONEXIONES.md).

Con el montaje revisado:

```powershell
.\carrito.cmd cargar Control_USB COM3
.\carrito.cmd monitor Control_USB COM3
```

En el monitor, escribir una orden y pulsar Enter:

| Orden | Resultado |
|---|---|
| W | Avanzar |
| S | Retroceder |
| A / D | Girar a izquierda / derecha |
| X | Deshabilitar motores |
| + / - | Subir / bajar potencia PWM |
| ? | Consultar estado |

El programa arranca detenido y deshabilita motores tras **1,5 segundos sin
otra orden de movimiento**. Para mantenerlo en movimiento se deben repetir
órdenes W/S/A/D antes de ese plazo. Mantener una tecla pulsada no garantiza
que la terminal envíe los caracteres en ese momento. Las órdenes `+`, `-` y `?`
no prolongan el movimiento. Detener por software no impide la inercia.

Ctrl+C cierra el monitor: la parada por tiempo se producirá cuando venza el
plazo de la última orden, si el programa sigue ejecutándose correctamente.
Cerrar el monitor antes de volver a cargar un programa.

## Descargar cambios del equipo

Dentro de la carpeta del proyecto:

```powershell
git pull --ff-only
```

Actualizar Git no actualiza automáticamente el Arduino: volver a ejecutar
`cargar` para enviar la nueva versión. Si hay cambios locales y Git se niega
a actualizar, conservarlos y consultar al equipo; no usar `reset --hard`.

Para identificar la versión que están probando:

```powershell
git log -1 --oneline
```

Pueden descargar sin Git mediante **Code > Download ZIP** en GitHub. En ese
caso las actualizaciones se descargan de nuevo; `git pull` requiere un clon.

## Uso desde Linux (Ubuntu)

Instalar herramientas básicas una vez:

```bash
sudo apt update
sudo apt install git curl ca-certificates tar
```

Clonar e instalar:

```bash
git clone https://github.com/agustinvidal20004-sudo/carrito-arduino.git
cd carrito-arduino
bash carrito.sh instalar
bash carrito.sh puertos
```

Reemplazar `/dev/ttyUSB0` por el puerto detectado:

```bash
bash carrito.sh cargar Prueba_Placa /dev/ttyUSB0
bash carrito.sh monitor Prueba_Placa /dev/ttyUSB0
```

Si aparece `Permission denied`, revisar el grupo propietario del puerto.
En Ubuntu suele ser `dialout`. Agregar el usuario y cerrar sesión/volver a
entrar antes de intentar otra vez:

```bash
sudo usermod -aG dialout "$USER"
```

El instalador Linux incluido admite x86-64 y ARM64.

## Problemas frecuentes

- **`git` no reconocido:** instalar Git y reabrir la terminal.
- **`curl.exe` no reconocido en Windows:** usar una versión actual de Windows
  10/11 que lo incluya. No descargar ejecutables de sitios desconocidos.
- **No aparece puerto:** probar un cable USB de datos y revisar el Administrador
  de dispositivos de Windows. Si el conversor USB requiere controlador,
  identificar primero el chip; no instalar drivers al azar.
- **Puerto ocupado:** cerrar Arduino IDE, monitores serie y otras aplicaciones.
- **No sincroniza al cargar:** confirmar Uno, puerto y cable; desconectar lo que
  esté conectado a D0/D1 durante la carga. Un fallo no se resuelve necesariamente
  cambiando el programa.
- **Motores no giran:** revisar alimentación y conexiones; no aumentar voltaje
  sin confirmar especificaciones. El L298N introduce caída de tensión.

## Estado de verificación

Ver `docs/VERIFICACION.md` para distinguir compilación de pruebas físicas.
Los scripts descargan herramientas en `.tools/`, `.arduino/` y `.build/`.
Esas carpetas no forman parte del código que se comparte por Git.

## Fuentes oficiales

- Arduino CLI: https://arduino.github.io/arduino-cli/1.5/installation/
- Compilación: https://arduino.github.io/arduino-cli/1.5/commands/arduino-cli_compile/
- Carga: https://arduino.github.io/arduino-cli/1.5/commands/arduino-cli_upload/
- Git para Windows: https://git-scm.com/install/windows
- L298: https://www.st.com/resource/en/datasheet/l298.pdf

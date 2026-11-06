#!/usr/bin/env bash
set -euo pipefail
raiz=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
cli="$raiz/.tools/arduino-cli"
export ARDUINO_DIRECTORIES_DATA="$raiz/.arduino/data"
export ARDUINO_DIRECTORIES_DOWNLOADS="$raiz/.arduino/downloads"
export ARDUINO_DIRECTORIES_USER="$raiz/.arduino/user"
mkdir -p "$raiz/.arduino/tmp"
export TMPDIR="$raiz/.arduino/tmp"
accion=${1:-ayuda}
programa=${2:-Control_USB}
puerto=${3:-}
case "$programa" in
  Control_USB|Control_Bluetooth|Prueba_Bluetooth|Prueba_Placa|Prueba_Un_Sensor|Prueba_Sensores) ;;
  *) echo "Programa desconocido: $programa" >&2; exit 2 ;;
esac
if [[ "$accion" == instalar ]]; then
  mkdir -p "$raiz/.tools"
  if [[ ! -x "$cli" ]]; then
    case "$(uname -s):$(uname -m)" in
      Linux:x86_64) plataforma=Linux_64bit ;;
      Linux:aarch64|Linux:arm64) plataforma=Linux_ARM64 ;;
      *) echo 'Instalador preparado para Linux x86-64 o ARM64. Consulta Arduino CLI.' >&2; exit 2 ;;
    esac
    curl -fL --retry 3 "https://downloads.arduino.cc/arduino-cli/arduino-cli_1.5.1_${plataforma}.tar.gz" -o "$raiz/.tools/arduino-cli.tar.gz"
    tar -xzf "$raiz/.tools/arduino-cli.tar.gz" -C "$raiz/.tools" arduino-cli
    chmod +x "$cli"
  fi
  "$cli" version
  "$cli" core update-index
  "$cli" core install arduino:avr@1.8.8
  echo 'Instalacion terminada.'
  exit 0
fi
if [[ "$accion" == ayuda ]]; then
  echo 'Uso: bash carrito.sh instalar|puertos|compilar|cargar|monitor [PROGRAMA] [PUERTO]'
  echo 'Programas: Prueba_Placa, Prueba_Un_Sensor, Prueba_Sensores, Control_USB, Prueba_Bluetooth, Control_Bluetooth'
  exit 0
fi
[[ -x "$cli" ]] || { echo 'Primero ejecuta: bash carrito.sh instalar' >&2; exit 2; }
compilar() {
  "$cli" compile --fqbn arduino:avr:uno --build-path "$raiz/.build/$programa" "$raiz/$programa"
}
case "$accion" in
  puertos) "$cli" board list ;;
  compilar) compilar ;;
  cargar)
    [[ -n "$puerto" ]] || { echo "Falta puerto. Ejemplo: bash carrito.sh cargar $programa /dev/ttyUSB0" >&2; exit 2; }
    compilar
    "$cli" upload --fqbn arduino:avr:uno --port "$puerto" --input-dir "$raiz/.build/$programa"
    ;;
  monitor)
    [[ -n "$puerto" ]] || { echo "Falta puerto. Ejemplo: bash carrito.sh monitor $programa /dev/ttyUSB0" >&2; exit 2; }
    "$cli" monitor --port "$puerto" --config baudrate=9600
    ;;
  *) echo "Accion desconocida: $accion" >&2; exit 2 ;;
esac

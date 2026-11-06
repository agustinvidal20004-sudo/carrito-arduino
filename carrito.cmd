@echo off
setlocal
set "RAIZ=%~dp0"
set "CLI=%RAIZ%.tools\arduino-cli.exe"
set "ARDUINO_DIRECTORIES_DATA=%RAIZ%.arduino\data"
set "ARDUINO_DIRECTORIES_DOWNLOADS=%RAIZ%.arduino\downloads"
set "ARDUINO_DIRECTORIES_USER=%RAIZ%.arduino\user"
set "ACCION=%~1"
set "PROGRAMA=%~2"
set "PUERTO=%~3"
if "%ACCION%"=="" goto ayuda
if /i "%ACCION%"=="instalar" goto instalar
if /i "%ACCION%"=="ayuda" goto ayuda
if not exist "%CLI%" (
  echo Primero ejecuta: .\carrito.cmd instalar
  exit /b 2
)
if /i "%ACCION%"=="puertos" (
  "%CLI%" board list
  exit /b
)
if "%PROGRAMA%"=="" set "PROGRAMA=Control_USB"
if /i "%PROGRAMA%"=="Control_USB" goto programa_valido
if /i "%PROGRAMA%"=="Control_Bluetooth" goto programa_valido
if /i "%PROGRAMA%"=="Prueba_Bluetooth" goto programa_valido
if /i "%PROGRAMA%"=="Prueba_Placa" goto programa_valido
if /i "%PROGRAMA%"=="Prueba_Un_Sensor" goto programa_valido
if /i "%PROGRAMA%"=="Prueba_Sensores" goto programa_valido
echo Programa desconocido: "%PROGRAMA%"
exit /b 2
:programa_valido
if /i "%ACCION%"=="compilar" goto compilar
if /i "%ACCION%"=="cargar" goto cargar
if /i "%ACCION%"=="monitor" goto monitor
echo Accion desconocida: "%ACCION%"
exit /b 2
:instalar
if not exist "%RAIZ%.tools" mkdir "%RAIZ%.tools"
if exist "%CLI%" goto instalar_core
if /i "%PROCESSOR_ARCHITECTURE%"=="ARM64" (
  echo Este instalador automatico es para Windows x64. Consulta Arduino CLI para ARM64.
  exit /b 2
)
set "CARRITO_ARCHIVO=%RAIZ%.tools\arduino-cli.zip"
set "CARRITO_HERRAMIENTAS=%RAIZ%.tools"
curl.exe -fL --retry 3 "https://downloads.arduino.cc/arduino-cli/arduino-cli_1.5.1_Windows_64bit.zip" -o "%CARRITO_ARCHIVO%"
if errorlevel 1 exit /b 1
powershell.exe -NoProfile -Command "$ErrorActionPreference='Stop'; Expand-Archive -LiteralPath $env:CARRITO_ARCHIVO -DestinationPath $env:CARRITO_HERRAMIENTAS -Force"
if errorlevel 1 exit /b 1
:instalar_core
"%CLI%" version
if errorlevel 1 exit /b 1
"%CLI%" core update-index
if errorlevel 1 exit /b 1
"%CLI%" core install arduino:avr@1.8.8
if errorlevel 1 exit /b 1
echo Instalacion terminada.
exit /b 0
:compilar
"%CLI%" compile --fqbn arduino:avr:uno --build-path "%RAIZ%.build\%PROGRAMA%" "%RAIZ%%PROGRAMA%"
exit /b
:cargar
if "%PUERTO%"=="" goto falta_puerto
call :compilar
if errorlevel 1 exit /b 1
"%CLI%" upload --fqbn arduino:avr:uno --port "%PUERTO%" --input-dir "%RAIZ%.build\%PROGRAMA%"
exit /b
:monitor
if "%PUERTO%"=="" goto falta_puerto
"%CLI%" monitor --port "%PUERTO%" --config baudrate=9600
exit /b
:falta_puerto
echo Falta puerto. Ejemplo: .\carrito.cmd %ACCION% %PROGRAMA% COM3
exit /b 2
:ayuda
echo Uso: .\carrito.cmd instalar^|puertos^|compilar^|cargar^|monitor [PROGRAMA] [PUERTO]
echo Programas: Prueba_Placa, Prueba_Un_Sensor, Prueba_Sensores, Control_USB, Prueba_Bluetooth, Control_Bluetooth
exit /b 0

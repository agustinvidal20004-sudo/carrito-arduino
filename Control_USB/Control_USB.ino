#include <Arduino.h>

// Arduino Uno + L298N. Canal A: motor izquierdo; canal B: derecho.
// Pines propuestos; confirmar el cableado y la alimentacion antes de probar.
// Retirar puentes ENA/ENB para controlar velocidad con PWM.
// El puente 5V-EN es distinto: su configuracion queda pendiente.
const byte ENA = 5;
const byte IN1 = 7;
const byte IN2 = 8;
const byte ENB = 6;
const byte IN3 = 9;
const byte IN4 = 10;

// Cambiar a true si una rueda gira al reves al ordenar avanzar.
const bool INVERTIR_IZQUIERDO = false;
const bool INVERTIR_DERECHO = false;
const unsigned long TIEMPO_SIN_ORDEN_MS = 1500;
// Pausa de conmutacion; no garantiza que las ruedas ya se hayan detenido.
const unsigned long PAUSA_CAMBIO_MS = 100;

enum Movimiento : byte { PARADO, ADELANTE, ATRAS, IZQUIERDA, DERECHA };
// Prototipo explicito: evita que el preprocesador lo coloque antes del enum.
void aplicarMovimiento(Movimiento movimiento);
Movimiento solicitado = PARADO;
Movimiento aplicado = PARADO;
byte potencia = 120;  // PWM 0..255, no es una velocidad medida.
unsigned long ultimaOrden = 0;
unsigned long ultimaParada = 0;
bool actualizarPotencia = false;

void apagarMotores() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void escribirMotor(byte enable, byte a, byte b, int valor, bool invertir) {
  if (invertir) valor = -valor;
  digitalWrite(a, valor > 0 ? HIGH : LOW);
  digitalWrite(b, valor < 0 ? HIGH : LOW);
  analogWrite(enable, valor < 0 ? -valor : valor);
}

void aplicarMovimiento(Movimiento movimiento) {
  int izquierdo = 0;
  int derecho = 0;
  switch (movimiento) {
    case ADELANTE: izquierdo = potencia; derecho = potencia; break;
    case ATRAS: izquierdo = -potencia; derecho = -potencia; break;
    // Giros suaves: una rueda detenida y la otra hacia delante.
    case IZQUIERDA: derecho = potencia; break;
    case DERECHA: izquierdo = potencia; break;
    default: break;
  }
  escribirMotor(ENA, IN1, IN2, izquierdo, INVERTIR_IZQUIERDO);
  escribirMotor(ENB, IN3, IN4, derecho, INVERTIR_DERECHO);
}

void parar(const __FlashStringHelper *motivo) {
  apagarMotores();
  solicitado = PARADO;
  aplicado = PARADO;
  ultimaParada = millis();
  actualizarPotencia = false;
  Serial.println(motivo);
}

void mostrarEstado() {
  Serial.print(F("Movimiento solicitado (0=parado,1=avanza,2=atras,3=izq,4=der): "));
  Serial.print((byte)solicitado);
  Serial.print(F(" | PWM: "));
  Serial.println(potencia);
}

void procesarOrden(char orden) {
  if (orden >= 'a' && orden <= 'z') orden -= 'a' - 'A';
  if (orden == '\r' || orden == '\n' || orden == ' ' || orden == '\t') return;

  switch (orden) {
    case 'W': solicitado = ADELANTE; break;
    case 'S': solicitado = ATRAS; break;
    case 'A': solicitado = IZQUIERDA; break;
    case 'D': solicitado = DERECHA; break;
    case 'X': parar(F("STOP: orden X")); return;
    case '+':
      potencia = potencia > 235 ? 255 : potencia + 20;
      actualizarPotencia = true;
      mostrarEstado();
      return;
    case '-':
      potencia = potencia < 20 ? 0 : potencia - 20;
      actualizarPotencia = true;
      mostrarEstado();
      return;
    case '?': mostrarEstado(); return;
    default: parar(F("STOP: comando no reconocido")); return;
  }
  // Solo W/S/A/D renuevan el permiso temporal de movimiento.
  ultimaOrden = millis();
  mostrarEstado();
}

void actualizarMotores() {
  unsigned long ahora = millis();
  if (solicitado != PARADO && ahora - ultimaOrden >= TIEMPO_SIN_ORDEN_MS) {
    parar(F("STOP: 1,5 segundos sin orden de movimiento"));
    return;
  }

  if (solicitado != aplicado) {
    if (aplicado != PARADO) {
      apagarMotores();
      aplicado = PARADO;
      ultimaParada = ahora;
    }
    if (solicitado != PARADO && ahora - ultimaParada >= PAUSA_CAMBIO_MS) {
      aplicarMovimiento(solicitado);
      aplicado = solicitado;
      actualizarPotencia = false;
    }
  } else if (actualizarPotencia && aplicado != PARADO) {
    aplicarMovimiento(aplicado);
    actualizarPotencia = false;
  }
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  apagarMotores();
  ultimaParada = millis();
  Serial.begin(9600);
  Serial.println(F("Control USB listo. Arranque detenido."));
  Serial.println(F("W avanza | S atras | A izquierda | D derecha | X para"));
  Serial.println(F("+/- PWM | ? estado | repetir movimiento antes de 1,5 s"));
}

void loop() {
  // Un caracter por vuelta para atender tambien la parada por tiempo.
  if (Serial.available() > 0) procesarOrden((char)Serial.read());
  actualizarMotores();
}

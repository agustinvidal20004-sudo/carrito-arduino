#include <Arduino.h>
#include <SoftwareSerial.h>

// TXD del modulo -> D2 (RX del Uno).
// D3 es TX del Uno; NO conectar al RXD de 3,3 V sin adaptar nivel.
// Esta prueba solo recibe: RXD del modulo puede quedar desconectado.
SoftwareSerial bluetooth(2, 3);  // orden: RX del Arduino, TX del Arduino
const unsigned long BAUD_BLUETOOTH = 9600;

void setup() {
  // Desactivar los cuatro pines de motor del diagrama del tutorial.
  const byte pines[] = {5, 6, 9, 10};
  for (byte i = 0; i < sizeof(pines); ++i) {
    pinMode(pines[i], OUTPUT);
    digitalWrite(pines[i], LOW);
  }
  Serial.begin(9600);
  bluetooth.begin(BAUD_BLUETOOTH);
  Serial.println(F("Prueba Bluetooth D2/D3; mantener motores sin alimentacion."));
  Serial.println(F("Conecta la app. Se imprimen los bytes que envia por Bluetooth."));
}

void loop() {
  if (bluetooth.available() > 0) {
    byte dato = (byte)bluetooth.read();
    Serial.print(F("BT hex=0x"));
    if (dato < 16) Serial.print('0');
    Serial.print(dato, HEX);
    Serial.print(F(" decimal="));
    Serial.print(dato);
    if (dato >= 32 && dato <= 126) {
      Serial.print(F(" caracter='"));
      Serial.write(dato);
      Serial.print('\'');
    }
    Serial.println();
  }
}

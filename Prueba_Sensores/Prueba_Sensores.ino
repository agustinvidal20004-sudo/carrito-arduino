// Prueba de dos sensores con salida digital OUT.
// Izquierdo: OUT -> D2. Derecho: OUT -> D3. GND comun.
// Confirmar la tension admitida por los modulos antes de conectar VCC.
// No controla los motores. Monitor serie: 9600 baudios.

const byte SENSOR_IZQUIERDO = 2;
const byte SENSOR_DERECHO = 3;

void setup() {
  pinMode(SENSOR_IZQUIERDO, INPUT);
  pinMode(SENSOR_DERECHO, INPUT);
  Serial.begin(9600);
  Serial.println("Prueba de sensores: 0 y 1 son lecturas sin interpretar.");
  Serial.println("Anota las lecturas sobre blanco y sobre negro.");
}

void loop() {
  int izquierdo = digitalRead(SENSOR_IZQUIERDO);
  int derecho = digitalRead(SENSOR_DERECHO);

  Serial.print("Izquierdo: ");
  Serial.print(izquierdo);
  Serial.print(" | Derecho: ");
  Serial.println(derecho);

  delay(200);
}

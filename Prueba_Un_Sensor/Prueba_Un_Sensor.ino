// Un sensor: VCC -> 5V, GND -> GND, OUT -> D2.
// Monitor serie a 9600 baudios. No controla motores.
const byte SENSOR = 2;

void setup() {
  pinMode(SENSOR, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Prueba de un sensor en D2. Compara blanco y negro.");
}

void loop() {
  int lectura = digitalRead(SENSOR);
  digitalWrite(LED_BUILTIN, lectura);
  Serial.print("Sensor D2: ");
  Serial.println(lectura);
  delay(200);
}

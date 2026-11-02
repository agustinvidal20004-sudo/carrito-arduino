// Primera prueba: confirma que podemos cargar un programa en la placa.
// Para placas Arduino compatibles que tengan LED_BUILTIN.

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Placa lista para el seguidor de linea");
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}

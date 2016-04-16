#define LM35DZ_PIN A0

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  uint8_t i = 0;
  uint16_t adcUnits = 0;
  float temperature  = 0;
  for (i = 0; i < 9; i++) {
    adcUnits = analogRead(LM35DZ_PIN);
    temperature += adcUnits * 0.00488 * 100;
  }
  temperature /= 10;
  Serial.print("Temperature: ");
  Serial.println(temperature);

  delay(5000);
}

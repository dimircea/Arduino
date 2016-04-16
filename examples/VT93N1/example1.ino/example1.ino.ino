#define VT93N1_PIN A1

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  double voltage = 0, resistance = 0, 
       inputVoltage = 5, resistance2 = 10000,
       luxValue = 0;
  // collect 10 samples and average to obtain stable value.
  for ( byte i = 0; i < 10; i++) {
    // read voltage from pin, in Volts
    voltage += analogRead(VT93N1_PIN) * 0.00488;
  }
  voltage = voltage / 10.0;
  // calculate resistance R = R2 x (Vin / Vread - 1)
  resistance = resistance2 * ( inputVoltage / voltage - 1);
  // convert resistance value to LUX value for this sensor based in formula:
  // E = 341.64 / R^(10 / 9), where R is measured in kOhm
  luxValue = 341.64 / pow( resistance / 1000.0, 10.0 / 9.0);

  Serial.println("LUX: ");
  Serial.println(luxValue);

  delay(5000);
}

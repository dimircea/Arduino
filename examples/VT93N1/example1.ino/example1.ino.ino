#define VT93N1_PIN A1

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  double vAcrossR2 = 0, r1 = 0, luxValue = 0,
         vInput = 5, r2 = 10000;
  
  // collect 10 samples and average to obtain stable value.
  for ( byte i = 0; i < 10; i++) {
    // read voltage from pin, in Volts
    vAcrossR2 += analogRead(VT93N1_PIN) * 0.00488;
  }
  vAcrossR2 = vAcrossR2 / 10.0;

  // calculate resistance r1 = 22 * (vInput / vAcrossR1 - 1)
  r1 = r2 * ( vInput / vAcrossR2 - 1);

  // convert resistance value to LUX value for this sensor based in formula:
  // E = 341.64 / R^(10 / 9), where R is measured in kOhm
  luxValue = 341.64 / pow( r1 / 1000.0, 10.0 / 9.0);

  Serial.print("LUX: ");
  Serial.println(luxValue);

  delay(5000);
}

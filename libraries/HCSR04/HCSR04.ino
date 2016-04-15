#include "HCSR04.h"
#define TRIGGER_PIN 6
#define ECHO_PIN 5

HCSR04 hcsr04(TRIGGER_PIN, ECHO_PIN);

void setup() {
  // Start serial communication, used to show
  // sensor data in the Arduino serial monitor.
  Serial.begin(115200);
  // Wait for the HCSR04 sensor to settle.
  // This usually needs about one second...
  delay(1000);
}

void loop() {
  float distance = hcsr04.read(HCSR04::MetricsEL::mm);  
  if (distance > 0) {
    Serial.println(distance);
  } else {
    Serial.println("Error reading the sensor!");
  }
  // continuously read the sensor, ~10 times/s
  // Note: may be much less than ~10 times/s because reading
  // the sensor and checking the timeout may take up to 50ms
  delay(100);
}

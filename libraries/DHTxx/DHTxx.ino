#include "DHTxx.h"
#define DHT_PIN 7

Dht dht(DHT_PIN, Dht::TypeEL::DHT11);
// OR
// Dht dht(DHT_PIN, Dht::TypeEL::DHT21);
// OR
// Dht dht(DHT_PIN, Dht::TypeEL::DHT22);

void setup() {  
  // Start serial communication, used to show
  // sensor data in the Arduino serial monitor.
  Serial.begin(115200);
  // Wait for the DHT sensor to settle.
  // This may take a few seconds...
  delay(2500);
};

void loop() {
  // read data from the DHT sensor
  Dht::Result result = dht.read();
  // display data via the serial port. 
  // Use "Tools > Serial Monitor" to view the data.
  if (result.status == Dht::StatusEL::OK) {
    Serial.print("Temperature: ");
    Serial.println(result.temperature);
    Serial.print("Humidity: ");
    Serial.println(result.humidity);
  } else if (result.status == Dht::StatusEL::CRC_ERROR) {
    Serial.println("CRC error! ");
  } else {
    Serial.println("Timeout error! ");
  }
  // wait 5 seconds until the next reading
  delay(5000);
};

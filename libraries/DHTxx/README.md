### DHTxx Library
Allows to use DHTxx (XX= {11, 21, 22}) humidity and temperature sensor with almost any Arduino board.

### Required resources
The library has a flash footprint of about 2.1Kb and a RAM footprint of 15B, for one single instance (which is probably what you'll use most of the time).

### How to use
```
#include "DHTxx.h"
#define DHT_PIN 7 // change to whatever pin you want to use

Dht dht(DHT_PIN, Dht::TypeEL::DHT11);
   OR
Dht dht(DHT_PIN, Dht::TypeEL::DHT21);
   OR
Dht dht(DHT_PIN, Dht::TypeEL::DHT22);
```

Then in the `loop` method you can do:

```
Dht::Result result = dht.read();

if (result.status == Dht::StatusEL::OK) {
  // use result.temperature and result.humidity values...
} else {
  // do something to take care of the error...
}
```

### Example
```
#include "DHTxx.h"
#define DHT_PIN 7

Dht dht(DHT_PIN, Dht::TypeEL::DHT11);

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
```


### License
This code is released under [CC BY 4.0](http://creativecommons.org/licenses/by/4.0/) license.

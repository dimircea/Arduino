### HCSR04 Library
Allows to use HCSR04 ultrasonic distance measurement sensor with 'almost' any Arduino board.

### Sensors Shape
![HC-SR04 Sensor](https://github.com/dimircea/Arduino/blob/master/libraries/HCSR04/docs/media/HC-SR04.png?raw=true "HC-SR04 Sensor")

### Sensors Datasheet
 * [Download HC-SR04 Datasheet as PDF](https://github.com/dimircea/Arduino/blob/master/libraries/HCSR04/docs/HC-SR04.pdf)

### Required Arduino resources
The library has a flash footprint of about 1.3Kb and a RAM footprint of 3 Bytes, for one single instance. One instance is used for one sensor.

### How to use
```
#include "HC-SR04.h"
#define TRIGGER_PIN 6
#define ECHO_PIN 5

HCSR04 hcsr04(TRIGGER_PIN, ECHO_PIN);
```

Then in the `loop` method you can do:

```
float distance = hcsr04.read();  // returns distance in centimeters
// OR
// float distance = hcsr04.read(HCSR04::MetricsEL::mm);  // returns distance in millimeters 
// OR
// float distance = hcsr04.read(HCSR04::MetricsEL::m);  // returns distance in meters 
// OR
// float distance = hcsr04.read(HCSR04::MetricsEL::km);  // returns distance in kilometers

if (distance > 0) {
  // we've got an obstacle at the specified distance
} else {
  // no obstacle in the sensor range
}
```

### Example
```
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
};

void loop() {
  float distance = hcsr04.read();  
  if (distance > 0) {
    Serial.println(distance);
  } else {
    Serial.println("Error reading the sensor!");
  }
  // continuously read the sensor, ~10 times/s
  // Note: may be much less than ~10 times/s because reading
  // the sensor and checking the timeout may take up to 50ms
  delay(100);
};
```


### License
This code is released under [CC BY 4.0](http://creativecommons.org/licenses/by/4.0/) license.

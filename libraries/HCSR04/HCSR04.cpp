#include "HCSR04.h"

/**
 * Constructor.
 * @param triggerPin
 *          the Arduino pin connected to the trigger pin of the sensor.
 * @param echoPin
 *          the Arduino pin connected to the echo pin of the sensor.
 */
HCSR04::HCSR04(unsigned char triggerPin, unsigned char echoPin) {
  // store the pin used to trigger sensor reading
  this->triggerPin = triggerPin;
  // store the pin used to read data from sensor
  this->echoPin = echoPin;
  // the trigger pin is set to OUTPUT - is used to inquire data 
  // from sensor by sending a 10uS pulse to the trigger sensor pin
  pinMode(triggerPin, OUTPUT);
  // by default the trigger pin should be LOW
  digitalWrite(triggerPin, LOW);
  // the echo pin is used to capture data, so it is an INPUT
  pinMode(echoPin, INPUT);
}; 

/**
 * Reads sensor data. 
 * @param unit
 *        measurement unit, value of HCSR04::MetricsEL
 * @return distance in provided measurement unit, 
 *         or -1 if error occured
 */
float HCSR04::read(HCSR04::MetricsEL unit) {
  float d = 0;
  // The sensor requires a 10uS HIGH pulse 
  // to aknoledge that we want a reading.
  // We use 20uS, just in case
  digitalWrite(this->triggerPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(this->triggerPin, LOW);
  // Detect the length of the HIGH pulse from the echo pin, 
  // to find out the time needed for the signal sound 
  // to travel forth and back from the obstacle.
  d = pulseIn(this->echoPin, HIGH);
  // do the math and compute the distance in cm
  d = d / 58.2; // distance (in cm) = (pulseTime / 2) / 29.1
  // transform in the required measurement unit
  if (unit == MetricsEL::mm) {
    return d * 10;
  } else if (unit == MetricsEL::m) {
    return d / 100;
  } else if (unit == MetricsEL::km) {
    return d / 1000;
  } else {
    return d;
  }
};

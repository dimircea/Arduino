#ifndef HCSR04_h
#define HCSR04_h

#if ARDUINO < 100
#include <WProgram.h>
#include <pins_arduino.h>
#else
#include <Arduino.h>
#endif

class HCSR04 { 
  public:
    enum class MetricsEL: unsigned char {
      mm = 0,
      cm = 1,
      m = 2,
      km = 3
    };
    HCSR04(unsigned char triggerPin, unsigned char echoPin); 
    float read(HCSR04::MetricsEL unit);
    inline float read() {
      return read(HCSR04::MetricsEL::cm);
    };
  private:
    unsigned char triggerPin;
    unsigned char echoPin;
};
#endif

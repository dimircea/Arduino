#ifndef DHTxx_h
#define DHTxx_h

#if ARDUINO < 100
#include <WProgram.h>
#include <pins_arduino.h>
#else
#include <Arduino.h>
#endif

class Dht { 
  public:
     // Sensor types.
    enum class TypeEL: unsigned char {
      DHT11 = 16,
      DHT21 = 32,
      DHT22 = 64
    };
    // Sensor reading statuses.
    enum class StatusEL: unsigned char {
      // NONE ==> no reading performed yet
      NONE = 0,
      // OK ==> valid temperature and humidity values
      OK = 2,
      // CRC Error ==> better do another reading or check your sensor connection
      CRC_ERROR = 4,
      // TIMEOUT ==> the sensor does not communicate, check the connections and the sensor
      TIMEOUT = 8
    };
    /** 
     * Constructor: create a class instance.
     * @param pin
     *    the Arduino pin number used as communication pin with the DHTxx sensor.
     * @param type   
     *    one of the TypeEL::xxx literals (to distinguish between the sensors: DHT11, DHT22, etc)
     */
    Dht(unsigned char pin, TypeEL type) {
      this->pin = pin;
      this->type = type;
      // Initially we need to send request command 
      // to the sensor, so the pin must be set to OUTPUT.
      pinMode(pin, OUTPUT);
      // Default, the data pin must be HIGH, normally being 
      // pulled up by a resistor...but just in case...
      digitalWrite(pin, HIGH);
      // store the initialization time
      this->timestamp = millis();
    };
    // Define data structure which is used to denote 
    // a reading from the sensor.  
    struct Result {
      // DHT11: temperature value is [0, 50]°C, accurracy is ±2°C
      // DHT21: temperature value is [-40, 80]°C, accurracy is ±0.5°C
      // DHT22: temperature value is [-40, 125]°C, accurracy is ±0.2°C
      float temperature = 0.0;
      // DHT11: humidity value is [20, 80]%, accurracy ±5%
      // DHT21: humidity value is [0, 100]%, accurracy ±3%
      // DHT22: humidity value is [0, 100]%, accurracy ±2%
      float humidity = 0.0;
      // Sensor status
      StatusEL status = StatusEL::NONE;
    };
    // read data from sensor 
    Result read();
  private:
    // the timestamp of the last reading, used to determine 
    // if the minimum 2 seconds delay between the readings 
    // was respected, if not the latest reading is returned 
    // instead of making abort  a new one
    unsigned long timestamp;
    unsigned char pin;
    char readDataByte();
    bool isReadyForData();
    bool checkCrc(unsigned char data[5]);
    Result result;
    TypeEL type;
};
#endif

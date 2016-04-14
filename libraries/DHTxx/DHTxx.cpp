#include "DHTxx.h"

/**
 * Read one byte of data from the sensor.
 * @return the read byte or -1 if timeouts occurred
 */
char Dht::readDataByte() {
  const uint8_t timeout = 85;
  unsigned char dataByte = 0;
  unsigned long startTime = 0, elapsedTime = 0;
  
  for (unsigned char i = 0; i < 8; i++) {
    // Before every data bit there is a "separation" expressed by 
    // a LOW signal with a length of approximately 50 microseconds.
    // Wait for the HIGH signal to appear, but no longer than 85 microseconds.
    // The 85 microseconds were deducted from experiments with various DHT11/22
    // sensors and various Arduino boards, and picked the value that works with all of them.
    startTime = micros();
    while (digitalRead(this->pin) == LOW) {
      // separation LOW signal exceeded the timeout...
      if (micros() - startTime > timeout) return -1;
    }
    
    // read data bit
    startTime = micros();
    while (digitalRead(this->pin) == HIGH) {
      elapsedTime = micros() - startTime;
      // data bit HIGH signal exceeded the timeout...
      if (elapsedTime > timeout) return -1;
    }
    
    // a bit "1" means a HIGH signal of about 70 microseconds.
    // We use the range (50, 85) to detect a bit "1", based on 
    // the experimentation with various Arduino boards as well as 
    // the experimentation with various DHT11/22 sensors.
    if (elapsedTime > 50) 
      dataByte += 1 << (7 - i);
    
    // a bit "0" means a HIGH signal of about 26-28 microseconds.
    // We use the range (0, 50] to detect a bit "0", based on 
    // the experimentation with various Arduino boards as well as 
    // the experimentation with various DHT11/22 sensors.
    else if (elapsedTime <= 50) 
      // this is not really required, but is here for better a documentation
      dataByte += 0 << (7 - i);
  }
  
  // we have a byte of data...
  return dataByte;
};

/**
 * Make a data request to the sensor and wait for a "command received" response. 
 * @return true if the sensor aknolwdges the "command received" event 
 *         false if the sensor seems to not respond to data request command.
 */
bool Dht::isReadyForData() {
  long elapsedTime = 0;
  // Notify sensor that we like to get data.
  // Action: keep data pin LOW for minimum 500 microseconds.
  // Note: we do that for 1 millisecond, just in case...
  pinMode(this->pin, OUTPUT);
  digitalWrite(this->pin, LOW);
  delay( 1);
  // The data pin is now set to INPUT, so it 
  // can receive data from the sensor.
  pinMode(this->pin, INPUT);
  elapsedTime = millis();
  // Wait for PULL DOWN signal from the sensor...
  // Normally, the sensor sends a LOW signal in a short time.
  // This means that it understood our "request data" command.
  while (digitalRead(this->pin) == HIGH) {
    // It may be that the sensor does not answer, and, which is a "Timeout" error.
    // We wait for no longer than 10 milliseconds, before we trigger a "Timeout" error.
    if ( millis() - elapsedTime > 10) {
      return false;
    }
  };
  // The sensor PULLED DOWN the data pin.
  // We just wait for PULL UP
  while (digitalRead(this->pin) == LOW);
  // The sensor PULLED UP the data pin.
  // We just wait for PULL DOWN
  while (digitalRead(this->pin) == HIGH);
  // sensor is ready to send the 40 bits data stream
  return true;
};

/**
 * Perform a CRC check-sum to determine if data transmission errors occurred.
 * @return true if no CRC error is found
 *         false if CRC check-sum fails.
 */
bool Dht::checkCrc(unsigned char data[5]) {
  unsigned char crcByte = data[4];
  unsigned int checksum = data[0] + data[1] + data[2] +data[3];
  if ((checksum & 0x00FF) == crcByte) {
    return true;
  } else {
    return false;}
};

/**
 * Reads data from the DHTxx sensor.
 * @return a structure containing temperature, humidity values 
 *         and the status (see Dht::StatueEL::xxx)
 */
Dht::Result Dht::read() {
  unsigned char byteNmr = 0, i = 0, integral = 0, decimal = 0, data[5];
  char dataByte = 0;

  // Check if at least two seconds passed from the last reading
  // If not, we return the latest known value which was correctly read.
  if (millis() - this->timestamp < 2000 && this->result.status == StatusEL::OK) {
    return this->result;
  }
  // store timestamp of the reading
  this->timestamp = millis();
  // the sensor seems to not respond to data requests.
  if (!this->isReadyForData()) {
    result.status = StatusEL::TIMEOUT;
    return result;
  }
  // Read the 5 data bytes from sensor
  for (i = 0; i < 5; i++) {
    dataByte = this->readDataByte();
    if ( dataByte != -1) {
      data[i] = dataByte;
    } else {
      result.status = StatusEL::TIMEOUT;
      return result;
    }
  }
  // check crc
  if (this->checkCrc(data)) {
    // DHT21 and DHT22
    if (this->type == TypeEL::DHT22 || this->type == TypeEL::DHT21) {
      result.humidity = (data[0] * 256 + data[1]) * 0.1;
      result.temperature = ((data[2] & 0x7F) * 256 + data[3]) * 0.1;
      // negative temperature
      if (data[2] & 0x80) {
        result.temperature = -result.temperature;
      }
    } 
    // DHT11
    else {
      result.humidity = data[0];
      result.temperature = data[2];
    }
    result.status = StatusEL::OK;
  } else {
    result.status = StatusEL::CRC_ERROR;
  }
  // return data pin to OUTPUT and set it HIGH
  pinMode(this->pin, OUTPUT);
  digitalWrite(this->pin, HIGH);
  // return sensor reading result
  return result;
};

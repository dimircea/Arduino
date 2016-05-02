# Arduino-ESP8266
Implement UART communication between Arduino boards and ESP8266 WiFi modules.

This library implements the AT commands required to communicate with the ESP8266 WiFi modules over UART/Serial port. 
It allows to select which Serial port to use for Adruino boards which have more than one, such as Arduino MEGA2560.

## Supported AT Commands
Currently, the following AT commands are supported:
* AT - `at` method;
* ATE0 - `ate0` method;
* ATE1 - `ate1` method;
* AT+RST - `atRst` method;
* AT+CIPSTART - for UDP, use `atCipstartUdp` method, which supports single and multiple link connections and allows to specify IP and port;
* AT+CIPCLOSE - `atCipclose` method, which supports single or multiple link connections;
* IDP - incomming data is supported via the ipd method, and supports single and multiple link connections;
* AT+other - comming soon.

Most of the above methods allows to specify a timeout before a communication fail/error is reported. 
Many methods have multiple signatures, with default values for some standard parameters.

## Installation
Clone this repo, rename the folder to ESP8266 and copy it under the `libraries` subfolder of your Arduino Software installation folder. 

## Usage Example
```
#include <ESP8266.h>

ESP8266 esp( Serial);
bool wifiActive = false;

void setup() {
  Serial.begin( 115200);
  // software reset for the ESP8266 module
  esp.atRst();
  // disable ECHO
  esp.at0();
  // check if module responds to AT commands
  wifiActive = (esp.at() == ESP8266::Error::NONE);
}

void loop() {
  char data[64] = {0}, *ipdData = data;
  // WiFi module inactive, check again....
  if ( wifiActive = (esp.at() == ESP8266::Error::NONE)) {
    if ( Serial.available() > 0) {
      if ( esp.ipd( ipdData) == ESP8266::Error::NONE) {
        // do something with the received bytes by using the ipdData pointer...
      } 
    }
  }
}
```

## Supported Arduino Boards:
This module was tested with Arduino UNO, MEGA2560, NANO and Pro Mini boards. However, it must work on most Arduino boards.
In case you find one which does not works, or possibly find a bug, please report and a fix will be released as soon as possible.

## Want to Contribute ?
You are welcome to contribute! Please contact me: dimircea[at]gmail.com.

## License
All the code and examples are available under the [GNU General Public License](http://www.gnu.org/licenses/gpl.html)

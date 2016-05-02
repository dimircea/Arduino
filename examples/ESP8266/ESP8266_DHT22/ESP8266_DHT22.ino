#include <DHTxx.h>
#include <ESP8266.h>
#define DHT_PIN 7

Dht dht(DHT_PIN, Dht::TypeEL::DHT22);
ESP8266 esp(Serial);

// WiFi authentication data
const char* WIFI_SSID = "wotap";
const char* WIFI_PASSWORD = "g3ma4ode";

// data server address and port
const char* DATA_SERVER_ADDRESS = "api.thingspeak.com"
const uint8_t DATA_SERVER_PORT = 80;

// data to be sent to the server
const char* DATA_SERVER_API_KEY = "4V3YD3FGLW5DR9MH";
char* DATA_SERVER_PATH = "/update";
const char DATA_TEMPLATE[] PROGMEM = "?api_key=%s&field1=%s&field2=%s";

boolean checkWiFi() {
  if( esp.at() == ESP8266::Error::NONE) return true; 
  return false;
};

void setup() {  
  // Start serial communication, used to 
  // communicate with the ESP8266 WiFi module.
  Serial.begin(115200);
  // connect to WiFi network...try again and again
  while (esp.atCwjap(WIFI_SSID, WIFI_PASSWORD) != ESP8266::Error::NONE);
};

void createDataFromTemplate( char *&data, float temperature, float humidity) {
  char buffTemp[6] = {0}, buffHum[5] = {0}, tmpl[32] = {0};
  char *pTmpl = tmpl;
  uint8_t templateLen = -1;
  // read template from PROGMEM
  getPMData( DATA_TEMPLATE, pTmpl, templateLen);
  // create data string from template by replacing
  // parameters with their actual values from sensors
  sprintf( data, pTmpl, DATA_SERVER_API_KEY,
    dtostrf( temperature, 0, 1, buffTemp),
    dtostrf( humidity, 0, 1, buffHum));
};

void sendDataToServer(float temperature, float humidity) {
  char data[64] = {0};
  char *pData = data;
  createDataFromTemplate(pData, temperature, humidity);
  esp.atCipstartTcp(DATA_SERVER_ADDRESS, 80);
  esp.atCipsendHttpGet(DATA_SERVER_PATH, data);
  esp.atCipclose();
};

void loop() {
  // nothing to do if the WiFi module does not 
  // responds to AT command...check the hardware! 
  if (!checkWiFi()) return;
  // read data from the DHT sensor
  Dht::Result result = dht.read();
  // if communication with the sensor was 
  // succesful, we send data to thingspeak.com
  if (result.status == Dht::StatusEL::OK) 
    sendDataToServer(result.temperature, result.humidity);
  // some delay until the next data sample 
  // is read and transmitted to server
  delay(5000);
};

#include <ESP8266.h>

// WiFi authentication data
const char* WIFI_SSID = "Gaudeamus";
const char* WIFI_PASSWORD = "1982gmus1478";

const char* SERVER_ADDRESS = "192.168.178.21";
char* SERVER_REGISTER_PATH = "/register/";
char* SERVER_DATA_PATH = "/data/";
char* SERVER_REGISTRATION_KEY = "team0";
char* SERVER_DATA_POST_PATH = "/data/team0";

ESP8266 esp(Serial);

void registerToServer(char* key) {
  esp.atCipstartTcp(SERVER_ADDRESS, 80);
  esp.clearSerialBuffer();
  esp.atCipsendHttpGet(SERVER_REGISTER_PATH, key);
  esp.clearSerialBuffer();
  esp.atCipclose();
  esp.clearSerialBuffer();
};

void postDataToServer(char* data) {
  esp.atCipstartTcp(SERVER_ADDRESS, 80);
  esp.clearSerialBuffer();
  esp.atCipsendHttpPost(SERVER_DATA_POST_PATH, data);
  esp.clearSerialBuffer();
  esp.atCipclose();
  esp.clearSerialBuffer();
};

void getDataFromServer() {
  esp.atCipstartTcp(SERVER_ADDRESS, 80);
  esp.clearSerialBuffer();
  esp.atCipsendHttpPost(SERVER_DATA_PATH, SERVER_REGISTRATION_KEY);
  esp.clearSerialBuffer();
  esp.atCipclose();
  esp.clearSerialBuffer();
};

void setup() {
  delay(5000);
  Serial.begin(115200);
  delay(1000);

  // software reset the ESP8266 WiFi module
  while(esp.atRst() != ESP8266::Error::NONE);
  // check if WiFi module is active
  while(esp.at() != ESP8266::Error::NONE);  
  // set station mode for WiFi module
  esp.atCwmode(ESP8266::WiFiMode::STA);
  // wait to connect to WiFi network
  while(esp.atCwjap(WIFI_SSID, WIFI_PASSWORD) != ESP8266::Error::NONE);
  // register to data server
  registerToServer(SERVER_REGISTRATION_KEY);


   char receivedData[256] = {0};
  char* prd = receivedData;
  uint16_t prdLen = 0;
  
  
  // post (store) some data to server
  postDataToServer("temperature=25");
 /* delay(2500);
  postDataToServer("humidity=58");
  delay(2500);
  // get data from server
  getDataFromServer();
  delay(1000);
  esp.ipd(prd, prdLen);
  Serial.println(prdLen);
  esp.clearSerialBuffer();
  // do something here with prd which contains the response data
  getDataFromServer();
  delay(1000);
  esp.ipd(prd, prdLen);
  // do something here with prd which contains the response data*/
};

void loop() {
 
};


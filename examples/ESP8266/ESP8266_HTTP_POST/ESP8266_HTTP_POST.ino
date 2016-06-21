#include <ESP8266.h>

// WiFi authentication data
const char* WIFI_SSID = "your-wifi-ssid";
const char* WIFI_PASSWORD = "<our-wifi-password";
const char* SERVER_ADDRESS = "your-server-ip";


char* SERVER_REGISTER_PATH = "/register/";
char* SERVER_DATA_PATH = "/data/";
char* SERVER_REGISTRATION_KEY = "team0";
char* SERVER_DATA_POST_PATH = "/data/team0";

ESP8266 esp(Serial);
SoftwareSerial debug(10, 11);

void setup() {
  char receivedData[256] = {0};
  char* prd = receivedData;
  uint16_t prdLen = 0;

  Serial.begin(115200);
  debug.begin(115200);
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
  esp.atCipstartTcp(SERVER_ADDRESS, 80);
  esp.atCipsendHttpGet(SERVER_REGISTER_PATH, SERVER_REGISTRATION_KEY);
  delay(1000);
  esp.atCipclose();
  
  // post (store) some data to server
  esp.atCipstartTcp(SERVER_ADDRESS, 80);
  esp.atCipsendHttpPost(SERVER_DATA_POST_PATH, "temperature=25");
  delay(1000);
  esp.atCipclose();

  // get data from server
  esp.atCipstartTcp(SERVER_ADDRESS, 80);
  esp.atCipsendHttpGet(SERVER_DATA_PATH, SERVER_REGISTRATION_KEY);
  
  // process incomming data
  esp.ipd(prd, prdLen, 2500);
  // do something with prd, and its length represented by prdLen
  debug.println(prdLen);
  debug.println(prd);
  esp.atCipclose();

  // NODE: due to async behavior of the send/receive http data, care
  // must be taken with timeouts and with deleting serial buffer!
};

void loop() {};


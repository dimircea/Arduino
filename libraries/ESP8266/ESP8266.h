#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "Util.h"
#include <SoftwareSerial.h>
#include <Arduino.h>

class ESP8266 {
  public:
    enum class Error {
      NONE = 0,
      TIMEOUT = 1,
      EMPTY_DATA,
      EMPTY_STREAM
    };
    enum class LinkId {
      ID_0 = 0,
      ID_1 = 1,
      ID_2 = 2,
      ID_3 = 3,
      ID_4 = 4,
      // this is used in only for some 
      // commands, such as AT+CIPCLOSE
      ALL = 5,
      // this is used for CIMUX = 0 case
      NONE = 6
    };
    enum class UdpMode {
      DESTINATION_FIXED = 0,
      DESTINATION_CHANGE_ONCE = 1,
      DESTINATION_DYNAMIC = 2
    };
    enum class Channel {
      CH_1 = 1,
      CH_2 = 2,
      CH_3 = 3,
      CH_4 = 4,
      CH_5 = 5,
      CH_6 = 6,
      CH_7 = 7,
      CH_8 = 8,
      CH_9 = 9,
      CH_10 = 10,
      CH_11 = 11
    };
    enum class WiFiMode: uint8_t {
      STA = 1,
      AP = 2,
      AP_STA = 3
    };
    enum class Encription {
      OPEN = 0,
      WPA_PSK = 2,
      WPA2_PSK = 3,
      WPA_WPA2_PSK = 4
    };
    ESP8266( HardwareSerial& ser): serial( ser){
      // allocate space for the longest command (32 chars + '\0')
      this->cmdData = "                                 ";
      this->cmdLen = 0;
      this->cTime = 0;
    };
    void clearSerialBuffer();
    Error at(uint16_t timeout = 500);
    Error ate0(uint16_t timeout = 500);
    Error ate1(uint16_t timeout = 500);
    Error atRst(uint16_t timeout = 2000);
    Error atCwmode(WiFiMode mode = WiFiMode::STA, 
      uint16_t timeout = 500);
    Error atCwsap(char* ssid, char* passwd, 
      Channel channel = Channel::CH_5, 
      Encription enc = Encription::WPA2_PSK, 
      uint16_t timeout = 2000);
    Error atCwjap(const char* ssid, const char* passwd,
      uint16_t timeout = 15000);
    Error atCipstartUdp(char* remoteIp = "0", uint16_t remotePort = 0, 
      uint16_t localPort = 1025, UdpMode mode = UdpMode::DESTINATION_DYNAMIC, 
      uint16_t timeout = 5000);
    Error atCipstartTcp(const char* remoteIp = "0", uint16_t remotePort = 0, 
      uint16_t timeout = 5000);
    Error atCipclose(LinkId linkId = LinkId::NONE, 
      uint16_t timeout = 1000);
    Error ipd(char *&data, uint16_t &dataLen, LinkId &linkId, 
      uint16_t waitTime = 0);
    inline Error ipd(char *&data, LinkId &linkId, uint16_t waitTime = 0) {
      uint16_t dataLen = 0;
      return this->ipd(data, dataLen, linkId);
    };
    inline Error ipd(char *&data, uint16_t &dataLen, uint16_t waitTime = 0) {
      ESP8266::LinkId linkId = ESP8266::LinkId::NONE;
      return this->ipd(data, dataLen, linkId);
    };
    /*inline Error ipd(char *&data, uint16_t waitTime = 0) {
      uint16_t dataLen = 0;
      ESP8266::LinkId linkId = ESP8266::LinkId::NONE;
      return this->ipd(data, dataLen, linkId);
    };*/
    
    Error atCipsend(char *data, LinkId linkId = LinkId::NONE, 
      uint16_t timeout = 1000);
    Error atCipsendHttpGet(char *path, char *data, 
      LinkId linkId = LinkId::NONE, uint16_t timeout = 1000);
    Error atCipsendHttpPost(char *path, char *data, 
      LinkId linkId = LinkId::NONE, uint16_t timeout = 1000);

  private:
    char *cmdData;
    uint8_t cmdLen;
    uint32_t cTime;
    HardwareSerial& serial;
    Error checkTimeout(const char* response, uint16_t timeout);
    inline Error checkTimeout(const char response, uint16_t timeout) {
      return this->checkTimeout((const char*) response, timeout);
    };
};
// constants stored in Program Memory (FLASH)
const char ESP8266_PGM_AT[] PROGMEM = "AT";
const char ESP8266_PGM_ATE0[] PROGMEM = "ATE0";
const char ESP8266_PGM_ATE1[] PROGMEM = "ATE1";
const char ESP8266_PGM_AT_RST[] PROGMEM = "AT+RST";
const char ESP8266_PGM_AT_CWMODE[] PROGMEM = "AT+CWMODE";
const char ESP8266_PGM_AT_RST_READY[] PROGMEM = "ready";
const char ESP8266_PGM_AT_CWSAP[] PROGMEM = "AT+CWSAP";
const char ESP8266_PGM_AT_CWJAP[] PROGMEM = "AT+CWJAP";
const char ESP8266_PGM_AT_CIPSTART[] PROGMEM = "AT+CIPSTART";
const char ESP8266_PGM_AT_CIPSTART_CONNECT_OK[] PROGMEM = "CONNECT\r\n\r\nOK\r\n";
const char ESP8266_PGM_AT_CIPCLOSE[] PROGMEM = "AT+CIPCLOSE";
const char ESP8266_PGM_AT_CIPCLOSE_CLOSED[] PROGMEM = "CLOSED";
const char ESP8266_PGM_IPD[] PROGMEM = "+IPD";
const char ESP8266_PGM_GOT_IP[] PROGMEM = "GOT IP";
const char ESP8266_PGM_AT_CIPSEND[] PROGMEM = "AT+CIPSEND";
const char ESP8266_PGM_AT_CIPSEND_SEND_OK[] PROGMEM = "SEND OK";
const char ESP8266_PGM_HTTP_VERSION[] PROGMEM = "HTTP/1.1";
const char ESP8266_PGM_HTTP_HEADER_CONTENT_TYPE[] PROGMEM = "Content-Type";
const char ESP8266_PGM_HTTP_HEADER_CONTENT_LENGTH[] PROGMEM = "Content-Length";
const char ESP8266_PGM_HTTP_HEADER_CONTENT_TYPE_FORM_URLENCODED[] PROGMEM = "application/x-www-form-urlencoded";
const char ESP8266_PGM_HTTP_GET[] PROGMEM = "GET";
const char ESP8266_PGM_HTTP_POST[] PROGMEM = "POST";
// constants stored in RAM
const char ESP8266_CR_LF[] = "\n";
const char ESP8266_CMD_LF[] = "\n";
const char ESP8266_CMD_END[] = "\r\n";
const char ESP8266_OK[] = "OK\r\n";
const char ESP8266_UDP[] = "UDP";
const char ESP8266_TCP[] = "TCP";
const char ESP8266_ZERO = '0';
const char ESP8266_COMA = ',';
const char ESP8266_DQUOTE = '"';
const char ESP8266_EQUAL = '=';
const char ESP8266_COLON = ':';
const char ESP8266_SEMI_COLON = ';';
const char ESP8266_WHITE_SPACE = ' ';
const char ESP8266_GREATER_THAN = '>';
#endif

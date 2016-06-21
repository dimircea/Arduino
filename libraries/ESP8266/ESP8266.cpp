/* 
 * Implement the ESP8266 class.
 *
 * @file ESP8266.cpp
 * @version 1.0 
 * @created 02.02.2015 11:35:54
 * @author: Mircea Diaconescu
 */ 
#include "ESP8266.h"
#include <Arduino.h>

/************************************************************************/
/* @method                                                              */
/* Utility method used internally to clear serial buffer                */
/************************************************************************/
void ESP8266::clearSerialBuffer() {
  while (this->serial.available())
    this->serial.read();
};

/************************************************************************/
/* @method                                                              */
/* Utility method used to detect command responde timeout               */
/* @param response                                                      */
/*         the responde to wait for                                     */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for response before gave up)                                */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::checkTimeout(const char* response, 
  uint16_t timeout) {
  
  // start recording elapsed time
  this->cTime = millis();
  // wait for response
  while((millis() - this->cTime) < timeout) 
    if (this->serial.available() >= 1 && this->serial.find((char*)response)) 
      return Error::NONE;
  // timeout error...
  return Error::TIMEOUT;
};

/************************************************************************/
/* @method                                                              */
/* Software check if ESP8266 module is ok: send AT command              */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for OK response before gave up)                             */
/*          NOTE: default value is 500                                  */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::at(uint16_t timeout) {
  getPMData(ESP8266_PGM_AT, this->cmdData, this->cmdLen);
  // send AT command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_CMD_END);
  return this->checkTimeout(ESP8266_OK, timeout);
};

/************************************************************************/
/* @method                                                              */
/* Disable echo: send ATE0 command                                      */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for OK response before gave up)                             */
/*          NOTE: default value is 500                                  */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::ate0(uint16_t timeout) {
  getPMData(ESP8266_PGM_ATE0, this->cmdData, this->cmdLen);
  // send ATE0 command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_CMD_END);
  return this->checkTimeout(ESP8266_OK, timeout);
};

/************************************************************************/
/* @method                                                              */
/* Enable echo: send ATE1 command                                       */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for OK response before gave up)                             */
/*          NOTE: default value is 500                                  */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::ate1(uint16_t timeout) {
  getPMData(ESP8266_PGM_ATE1, this->cmdData, this->cmdLen);
  // send ATE1 command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_CMD_END);
  return this->checkTimeout(ESP8266_OK, timeout);
};

/************************************************************************/
/* @method                                                              */
/* Software reset he ESP8266 module: send AT+RST command                */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for OK response before gave up)                             */
/*          NOTE: default value is 1000                                 */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atRst(uint16_t timeout) {
  getPMData(ESP8266_PGM_AT_RST, this->cmdData, this->cmdLen);
  // send AT+RST command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_CMD_END);
  getPMData(ESP8266_PGM_AT_RST_READY, this->cmdData, this->cmdLen);
  return this->checkTimeout(this->cmdData, timeout);
};


/************************************************************************/
/* @method                                                              */
/* Set the WiFi mode: send AT+CWMODE command                            */
/* @param mode                                                          */
/*          the WiFi mode (STA, AP or STA+AP), values of WiFiMode::xxx  */
/*          Defaults to STA.
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for OK response before gave up)                             */
/*          NOTE: default value is 500                                  */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atCwmode(WiFiMode mode, uint16_t timeout) {
  getPMData(ESP8266_PGM_AT_CWMODE, this->cmdData, this->cmdLen);
  // send AT+CWMODE=mode command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_EQUAL);
  this->serial.print((uint8_t)mode);
  this->serial.print(ESP8266_CMD_END);
  return this->checkTimeout(ESP8266_OK, timeout);
};

/************************************************************************/
/* @method                                                              */
/* Access point settings: execute AT+CWSAP                              */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for OK response before gave up)                             */
/*          NOTE: default value is 2000                                 */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atCwsap(char* ssid, char* passwd, 
  Channel channel, Encription enc, uint16_t timeout) {
    
  getPMData(ESP8266_PGM_AT_CWSAP, this->cmdData, this->cmdLen);
  // send AT+CWSAP command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_EQUAL);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ssid);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_COMA);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(passwd);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_COMA);
  this->serial.print((uint8_t)channel);
  this->serial.print(ESP8266_COMA);
  this->serial.print((uint8_t)enc);
  this->serial.print(ESP8266_CMD_END);
  return this->checkTimeout(ESP8266_OK, timeout);
};

/************************************************************************/
/* @method                                                              */
/* Client  settings: execute AT+CWJAP                                   */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for OK response before gave up)                             */
/*          NOTE: default value is 2000                                 */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atCwjap(const char* ssid, const char* passwd, 
  uint16_t timeout) {

  getPMData(ESP8266_PGM_AT_CWJAP, this->cmdData, this->cmdLen);
  // send AT+CWSAP command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_EQUAL);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ssid);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_COMA);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(passwd);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_CMD_END);
  return this->checkTimeout(ESP8266_OK, timeout);
};


/************************************************************************/
/* @method                                                              */
/* Execute AT+CIPSTART for UDP                                          */
/* @param remoteIp                                                      */
/*          the IP of the remote side for the udp connection            */
/* @param remotePort                                                    */
/*          the port of the remote side for the udp connection          */
/* @param localPort                                                     */
/*          the local port for the udp connection                       */
/* @param udpMode                                                       */
/*          the udp mode (see ESP8266::UdpMode::XXX                     */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for OK response before gave up)                             */
/*          NOTE: default value is 5000                                 */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atCipstartUdp(char* remoteIp, uint16_t remotePort, 
  uint16_t localPort, UdpMode mode, uint16_t timeout) {

  getPMData(ESP8266_PGM_AT_CIPSTART, this->cmdData, this->cmdLen);
  // send AT+CIPSTART command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_EQUAL);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_UDP);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_COMA);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(remoteIp);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_COMA);
  this->serial.print(remotePort);
  this->serial.print(ESP8266_COMA);
  this->serial.print(localPort);
  this->serial.print(ESP8266_COMA);
  this->serial.print((uint8_t) mode);
  this->serial.print(ESP8266_CMD_END);
  return this->checkTimeout(ESP8266_OK, timeout);
};

/************************************************************************/
/* @method                                                              */
/* Execute AT+CIPSTART for TCP                                          */
/* @param remoteIp                                                      */
/*          the IP of the remote side for the tcp connection            */
/* @param remotePort                                                    */
/*          the port of the remote side for the tcp connection          */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for OK response before gave up)                             */
/*          NOTE: default value is 5000                                 */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atCipstartTcp(const char* remoteIp, 
  uint16_t remotePort, uint16_t timeout) {

  getPMData(ESP8266_PGM_AT_CIPSTART, this->cmdData, this->cmdLen);
  // send AT+CIPSTART command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_EQUAL);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_TCP);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_COMA);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(remoteIp);
  this->serial.print(ESP8266_DQUOTE);
  this->serial.print(ESP8266_COMA);
  this->serial.print(remotePort);
  this->serial.print(ESP8266_CMD_END);
  getPMData(ESP8266_PGM_AT_CIPSTART_CONNECT_OK, this->cmdData, this->cmdLen);
  return this->checkTimeout(this->cmdData, timeout);
};

/************************************************************************/
/* @method                                                              */
/* Close TCP/UDP connection (AT+CIPCLOSE command)                       */
/* @param linkId                                                        */
/*          the link ID of the connection to close (LinkId::xxx values) */
/*          NOTE: skip this param when CIPMUX = 0. If CIMUX = 1, use    */ 
/*                the value LinkId::ID_ALL to close all connections.    */
/*                (CIPMUX must be 1) to close all connections.          */
/* @param timeout                                                       */
/*          timeout in milliseconds for this command ( the time to wait */
/*          for CLOSEDresponse before gave up)                          */
/*          NOTE: default value is 1000                                 */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atCipclose(LinkId linkId, uint16_t timeout) {
  getPMData(ESP8266_PGM_AT_CIPCLOSE, this->cmdData, this->cmdLen);
  // send AT+CIPCLOSE command
  this->serial.print(this->cmdData);
  if (linkId <= LinkId::ALL) {
    this->serial.print(ESP8266_EQUAL);
    this->serial.print((int)linkId);
  }
  this->serial.print(ESP8266_CMD_END);
  getPMData(ESP8266_PGM_AT_CIPCLOSE_CLOSED, this->cmdData, this->cmdLen);
  return this->checkTimeout(this->cmdData, timeout);
};

/************************************************************************/
/* @method                                                              */
/* Receive data (+IPD)                                                  */
/* @param data                                                          */
/*          reference parameter storing the received data               */
/* @param dataLen                                                        */
/*          reference parameter storing the received data length        */
/* @param linkId                                                        */
/*          link ID reference of the connection (LinkId::xxx values)    */
/*          NOTE: this is LinkId::NONE when CIPMUX = 0. If CIMUX = 1,   */ 
/*                the value is LinkId::ID_X (x = [0, 4]).               */
/* @param waitTime                                                      */
/*          timeout in milliseconds to wait for data (blocking!)        */
/*          NOTE: default value is 0                                    */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::ipd(char *&data, uint16_t &dataLen, 
  LinkId &LinkId, uint16_t waitTime) {
    
  uint8_t c = 0, len[4] = {0};
  uint16_t i = 0, n = 0;
  // be sure that the reference data length value is 0
  dataLen = 0;
  // ESP8266 command string is loaded from PROGMEM
  getPMData(ESP8266_PGM_IPD, this->cmdData, this->cmdLen);
  // wait for data ( the specified wite time value)
  waitTime++;
  delay(waitTime);
  // check serial buffer for response - minimum 8 chars: "+IPD,x:y" 
  if (this->serial.available() > 7) {
    if (!this->serial.find( this->cmdData)) return Error::EMPTY_DATA;
    // next char is 'coma', so just drop it
    if (this->serial.available() > 0) this->serial.read();
    else return Error::EMPTY_STREAM;
    // next 1 to 4 digits represents the data length (0-2048)
    while (this->serial.available() && ':' != ( c = this->serial.read())) 
      len[n++] = c;
    // compute data length value (the real numeric value)
    for (i = 0; i < n; i++) dataLen = dataLen * 10 + len[i] - '0';
    // now extract received data
    for (i = 0; i < dataLen; i++)
      if (this->serial.available()) 
        *(data + i) = this->serial.read();  
    *(data + i) = '\0';
  }
  return Error::NONE;
};

/************************************************************************/
/* @method                                                              */
/* Send TCP/UDP data (execute AT+CIPSEND command)                       */
/* @param data                                                          */
/*          data to send (must be \0 terminated!)                       */
/* @param linkId                                                        */
/*          the connection ID (obtained when AT+CIPSTART executed)      */
/*          NOTE: this must be LinkId::NONE (default value) if the      */
/*                value of CIPMUX = 0 and LinkId::ID_x if CIPMUX = 1    */
/* @param timeout                                                       */
/*          timeout in milliseconds to wait for SEND OK answer          */
/*          NOTE: default value is 2000                                 */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atCipsend( char *data, LinkId linkId, 
  uint16_t timeout) {
    
  Error error = Error::NONE;
  uint16_t dataLen = 0;
  char *pData = data;
  long remainingTimeout = 0;
  // compute lengt of the data to be sent
  while ((*(pData + dataLen++)) != 0);
  // ESP8266 command string is loaded from PROGMEM
  getPMData(ESP8266_PGM_AT_CIPSEND, this->cmdData, this->cmdLen);
  // data to be send is empty...
  if (dataLen < 1) return Error::EMPTY_DATA;
  // send AT+CIPSEND command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_EQUAL);
  this->serial.print(dataLen); 
  this->serial.print(ESP8266_CMD_END);
  // start recording elapsed time
  this->cTime = millis();
  // wait for OK
  if (this->checkTimeout( ESP8266_OK, timeout) != Error::NONE) return error;
  // wait for '>'
  remainingTimeout = timeout - (millis() - this->cTime);
  if (remainingTimeout < 0) return Error::TIMEOUT;
  error = this->checkTimeout(ESP8266_GREATER_THAN, remainingTimeout);
  if (error != Error::NONE) return error;
  // send data
  this->serial.print(data);
  this->serial.print(ESP8266_CMD_END);
  // wait for SEND OK
  remainingTimeout = timeout - (millis() - this->cTime);
  // ESP8266 "SEND OK" string is loaded from PROGMEM
  getPMData(ESP8266_PGM_AT_CIPSEND_SEND_OK, this->cmdData, this->cmdLen);
  if (remainingTimeout < 0) return Error::TIMEOUT;
  return this->checkTimeout(this->cmdData, remainingTimeout);
};

/************************************************************************/
/* @method                                                              */
/* Send HTTP GET request                                                */
/* @param data                                                          */
/*          data to send (must be \0 terminated!)                       */
/* @param linkId                                                        */
/*          the connection ID (obtained when AT+CIPSTART executed)      */
/*          NOTE: this must be LinkId::NONE (default value) if the      */
/*                value of CIPMUX = 0 and LinkId::ID_x if CIPMUX = 1    */
/* @param timeout                                                       */
/*          timeout in milliseconds to wait for SEND OK answer          */
/*          NOTE: default value is 2000                                 */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atCipsendHttpGet(char *path, char *data, 
  LinkId linkId, uint16_t timeout) {
    
  Error error = Error::NONE;
  uint16_t dataLen = 0, pathLen = 0;
  char *pData = data;
  long remainingTimeout = 0;
  // compute lengt of the data to be sent
  while ((*(pData + dataLen++)) != 0);
  dataLen--;
  // compute lengt of the path where to send
  pData = path;
  while ((*(pData + pathLen++)) != 0);
  pathLen--;
  // ESP8266 command string is loaded from PROGMEM
  getPMData(ESP8266_PGM_AT_CIPSEND, this->cmdData, this->cmdLen);
  // data to be send is empty...
  if (dataLen < 1) return Error::EMPTY_DATA;
  // send AT+CIPSEND command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_EQUAL);
  // 17 = length(ESP8266_HTTP_HEADER + ESP8266_HTTP_GET + separator spaces)
  this->serial.print(pathLen + dataLen + 17);   
  this->serial.print(ESP8266_CMD_END);
  // start recording elapsed time
  this->cTime = millis();
  // wait for OK
  if (this->checkTimeout(ESP8266_OK, timeout) != Error::NONE) return error;
  // wait for '>'
  remainingTimeout = timeout - (millis() - this->cTime);
  if (remainingTimeout < 0) return Error::TIMEOUT;
  error = this->checkTimeout(ESP8266_GREATER_THAN, remainingTimeout);
  if (error != Error::NONE) return error;
  // send data
  // ESP8266 command string is loaded from PROGMEM
  getPMData(ESP8266_PGM_HTTP_GET, this->cmdData, this->cmdLen);
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_WHITE_SPACE);
  this->serial.print(path);
  this->serial.print(data);
  this->serial.print(ESP8266_WHITE_SPACE);
  getPMData(ESP8266_PGM_HTTP_VERSION, this->cmdData, this->cmdLen);
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_CR_LF);
  this->serial.print(ESP8266_CR_LF);
  this->serial.print(ESP8266_CMD_END);
  // wait for SEND OK
  remainingTimeout = timeout - (millis() - this->cTime);
  // ESP8266 "SEND OK" string is loaded from PROGMEM
  getPMData(ESP8266_PGM_AT_CIPSEND_SEND_OK, this->cmdData, this->cmdLen);
  if (remainingTimeout < 0) return Error::TIMEOUT;
  return this->checkTimeout(this->cmdData, remainingTimeout);
};

/************************************************************************/
/* @method                                                              */
/* Send HTTP GET request                                                */
/* @param data                                                          */
/*          data to send (must be \0 terminated!)                       */
/* @param linkId                                                        */
/*          the connection ID (obtained when AT+CIPSTART executed)      */
/*          NOTE: this must be LinkId::NONE (default value) if the      */
/*                value of CIPMUX = 0 and LinkId::ID_x if CIPMUX = 1    */
/* @param timeout                                                       */
/*          timeout in milliseconds to wait for SEND OK answer          */
/*          NOTE: default value is 2000                                 */
/* @return ESP8266::Error_NONE if all OK, ESP8266::Error::XXX otherwise */
/************************************************************************/
ESP8266::Error ESP8266::atCipsendHttpPost(char *path, char *data, 
  LinkId linkId, uint16_t timeout) {
    
  Error error = Error::NONE;
  uint16_t dataLen = 0, pathLen = 0, baseLen = 85;
  char *pData = data;
  long remainingTimeout = 0;
  // compute lengt of the data to be sent
  while ((*(pData + dataLen++)) != 0);
  dataLen--;
  // data to be send is empty...
  if (dataLen < 1) return Error::EMPTY_DATA;
  // compute lengt of the path where to send
  pData = path;
  while ((*(pData + pathLen++)) != 0);
  pathLen--;
  // add the number of chars used to represent the value of Content-length
  baseLen += String(dataLen).length();
  /**
   * a POST request example is shown below:
   *
   * POST /path HTTP/1.0\r\n
   * Content-Length: 14\r\n
   * Content-Type: application/x-www-form-urlencoded\r\n\r\n
   * temperature=25
   */
  //ESP8266 command string is loaded from PROGMEM
  getPMData(ESP8266_PGM_AT_CIPSEND, this->cmdData, this->cmdLen);
  // send AT+CIPSEND command
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_EQUAL);
  this->serial.print(pathLen + dataLen + baseLen);     
  this->serial.print(ESP8266_CMD_END);
  // start recording elapsed time
  this->cTime = millis();
  // wait for OK
  if (this->checkTimeout(ESP8266_OK, timeout) != Error::NONE) return error;
  // wait for '>'
  remainingTimeout = timeout - (millis() - this->cTime);
  if (remainingTimeout < 0) return Error::TIMEOUT;
  error = this->checkTimeout(ESP8266_GREATER_THAN, remainingTimeout);
  if (error != Error::NONE) return error;
  // send data
  // ESP8266 command string is loaded from PROGMEM
  getPMData(ESP8266_PGM_HTTP_POST, this->cmdData, this->cmdLen);
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_WHITE_SPACE);
  this->serial.print(path);
  this->serial.print(ESP8266_WHITE_SPACE);
  getPMData(ESP8266_PGM_HTTP_VERSION, this->cmdData, this->cmdLen);
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_CR_LF);
  getPMData(ESP8266_PGM_HTTP_HEADER_CONTENT_LENGTH, this->cmdData, this->cmdLen);
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_COLON);
  this->serial.print(ESP8266_WHITE_SPACE);
  this->serial.print(String(dataLen));
  this->serial.print(ESP8266_CR_LF);
  getPMData(ESP8266_PGM_HTTP_HEADER_CONTENT_TYPE, this->cmdData, this->cmdLen);
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_COLON);
  this->serial.print(ESP8266_WHITE_SPACE);
  getPMData(ESP8266_PGM_HTTP_HEADER_CONTENT_TYPE_FORM_URLENCODED, this->cmdData, this->cmdLen);
  this->serial.print(this->cmdData);
  this->serial.print(ESP8266_CR_LF);
  this->serial.print(ESP8266_CR_LF);
  this->serial.print(data);
  // wait for SEND OK
  remainingTimeout = timeout - (millis() - this->cTime);
  // ESP8266 "SEND OK" string is loaded from PROGMEM
  getPMData(ESP8266_PGM_AT_CIPSEND_SEND_OK, this->cmdData, this->cmdLen);
  if (remainingTimeout < 0) return Error::TIMEOUT;
  return this->checkTimeout(this->cmdData, remainingTimeout);
};

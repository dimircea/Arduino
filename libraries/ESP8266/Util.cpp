#include "Util.h"

/************************************************************************/
/* Calculate the current MCU free memory value (in bytes)               */
/* It works with MCUs up to 64KB RAM (uint16_t aka unsigned short type) */
/* @return the number of free RAM bytes available                       */
/************************************************************************/
uint16_t getFreeMCUMemory() {
  uint16_t free_memory;
  if ((uint16_t)__brkval == 0)
    return (((uint16_t)&free_memory) - ((uint16_t)&__bss_end));
  else
    return (((uint16_t)&free_memory) - ((uint16_t)__brkval));
};

/************************************************************************/
/* Extract data from PROGMEM                                            */
/* NOTE: the memory allocation has to be made outside (in the *data)    */
/* @param pmData                                                        */
/*          the PROGMEM data                                            */
/* @param resultData                                                    */
/*          a reference to a pointer (with enough allocated memory)     */ 
/*          the PROGMEM data read from flash is stored                  */
/*          NOTE: an '\0' is added at the end!                          */
/* @param length                                                        */
/*          reference parameter which after the method call will have   */
/*          as value the number of bytes found for pmData               */
/*          NOTE: value is NOT (length + 1) because of the added '\0'   */
/************************************************************************/
void getPMData(const char pmData[], char *&resultData, uint8_t &length) {
  char c = 0;
  const char *data = pmData;
  // load the data from PROGMEM and store it in resultData
  length = 0;
  while (0 != (c = pgm_read_byte(data++))) *(resultData + length++) = c;
  *(resultData + length) = '\0';
};

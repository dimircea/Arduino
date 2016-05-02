/*
 * Define a collections of methods and utilities
 * which can be used with many applications.
 *
 * @file Util.h
 * @version 1.0
 * @created 16.10.2015 14:31:17
 * @author: Mircea Diaconescu
 */ 

#ifndef UTIL_H_
#define UTIL_H_

#include <stdlib.h>
#include <stdint.h>
#include <avr/pgmspace.h>

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

uint16_t getFreeMCUMemory();
void getPMData( const char pmData[], char *&resultData, uint8_t &length);
int stringToInt( char *string);

#endif

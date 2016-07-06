/**
 * @file nonstdlib.h
 * @brief Provides common C functions that are not part of stdlib.h but
 * the Arduino depends upon.
 *
 * Copyright (c) 2013 by Martin Heermance
 * MIT Licensed
 */

#ifndef _NONSTDLIB_H
#define _NONSTDLIB_H

#ifdef __cplusplus
extern "C"{
#endif

void strreverse(char* begin, char* end);

void itoa(int value, char* str, int base);

void utoa(unsigned int value, char* str, int base);

void ltoa(long value, char* str, int base);

void ultoa(unsigned long value, char * str, int base);

#ifdef __cplusplus
}
#endif

#endif
// _NONSTDLIB_H

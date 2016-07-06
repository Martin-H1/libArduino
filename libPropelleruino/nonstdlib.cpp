/**
 * @file nonstdlib.cpp
 * @brief Provides common C functions that are not part of stdlib.h but
 * the Arduino depends upon.
 *
 * Copyright (c) 2013 by Martin Heermance
 * MIT Licensed
 */

#include <stdlib.h>
#include <ctype.h>
#include "nonstdlib.h"

// Amazingly itoa and utoa are not part of the C standard and thus not part
// of GCC. They were added in the AVR port, so they are included here.

/**
 * Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C"
 * with slight modification to optimize for specific architecture:
 */
void strreverse(char* begin, char* end)
{
	char aux;
	
	while(end>begin)
		aux=*end, *end--=*begin, *begin++=aux;
}

static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";	

void itoa(int value, char* str, int base)
{	
	char* wstr = str;
	int sign;
	div_t res;
	
	// Validate base
	if (base<2 || base>35)
  {
    *wstr='\0';
    return;
  }	
	
	// Take care of sign	
	if ((sign=value) < 0)
    value = -value;	
	
	// Conversion. Number is reversed.	
	do
  {	
		res = div(value,base);	
		*wstr++ = num[res.rem];	
	} while (value=res.quot);

	if (sign<0)
    *wstr++='-';
	
	*wstr='\0';
	
	// Reverse string	
	strreverse(str, wstr-1);
}

void utoa(unsigned int value, char* str, int base)
{
	char* wstr = str;
	div_t res;
	
	// Validate base
	if (base<2 || base>35)
  {
    *wstr='\0';
    return;
  }	
	
	// Conversion. Number is reversed.	
	do
  {	
		res = div(value,base);	
		*wstr++ = num[res.rem];	
	} while (value=res.quot);

	*wstr='\0';
	
	// Reverse string	
	strreverse(str, wstr-1);
}

void ltoa(long value, char* str, int base)
{
	char* wstr = str;
	long sign;
	div_t res;
	
	// Validate base
	if (base<2 || base>35)
  {
    *wstr='\0';
    return;
  }	
	
	// Take care of sign	
	if ((sign=value) < 0)
    value = -value;	
	
	// Conversion. Number is reversed.	
	do
  {	
		res = div(value,base);	
		*wstr++ = num[res.rem];	
	} while (value=res.quot);

	if (sign<0)
    *wstr++='-';
	
	*wstr='\0';
	
	// Reverse string	
	strreverse(str, wstr-1);
}

void ultoa(unsigned long value, char * str, int base)
{
	char* wstr = str;
	div_t res;
	
	// Validate base
	if (base<2 || base>35)
  {
    *wstr='\0';
    return;
  }	

	// Conversion. Number is reversed.	
	do
  {	
		res = div(value,base);	
		*wstr++ = num[res.rem];	
	} while (value=res.quot);

	*wstr='\0';
	
	// Reverse string	
	strreverse(str, wstr-1);
}

/*
  wiring_shift.c - shiftOut() function
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.c 248 2007-02-03 15:36:30Z mellis $
*/

#include "Arduino.h"

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
	uint8_t value = 0;
	uint8_t i;

	for (i = 0; i < 8; ++i) {
		digitalWrite(clockPin, HIGH);
		if (bitOrder == LSBFIRST)
			value |= digitalRead(dataPin) << i;
		else
			value |= digitalRead(dataPin) << (7 - i);
		digitalWrite(clockPin, LOW);
	}
	return value;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, !!(val & (1 << i)));
		else	
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));
			
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);		
	}
}

/*
inline int32_t Shr__(uint32_t a, uint32_t b) { return (a>>b); }

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
  int32_t	x;
  x = 0;
  do
  {
    if (bitOrder == MSBFIRST)
    {
      OUTA = ((OUTA & (~(1 << dataPin))) | ((((Shr__(val, (7 - x))) & 0x1) & 0x1) << dataPin));
    }
    else
    {
      OUTA = ((OUTA & (~(1 << dataPin))) | ((((Shr__(val, x)) & 0x1) & 0x1) << dataPin));
    }
    OUTA = ((OUTA & (~(1 << clockPin))) | (1 << clockPin));
    OUTA &= ~(1<<clockPin);
    x++;
  } while (x <= 7);
}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
  int32_t	x;
  uint8_t result = 0;
  x = 0;
  do {
    if (bitOrder == MSBFIRST)
    {
      result = (result | (((INA >> dataPin) & 0x1) << (7 - x)));
    }
    else
    {
      result = (result | (((INA >> dataPin) & 0x1) << x));
    }
    OUTA = ((OUTA & (~(1 << clockPin))) | (1 << clockPin));
    OUTA &= ~(1<<clockPin);
    x++;
  } while (x <= 7);
  return result;
}
*/
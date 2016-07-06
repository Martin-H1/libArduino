/**
 * @file wiring.c
 * @brief Provides Arduino types and functions on the Propeller
 *
 * Copyright (c) 2013 by Martin Heermance
 * MIT Licensed
 */

#include <Arduino.h>

static const int Ms = 1000;

// returns uptime in milliseconds. It is defined in terms
// of microseconds to handle all the hard stuff in one place.
unsigned long millis(void)
{
    return micros() / Ms;
}

// returns uptime in microseconds. Uses the CNT register and
// tracks rollover.
unsigned long micros(void)
{

// Time  = CNT / (CLKFREQ / Ms)
// cycles / (cycles / sec) 
}

void delay(unsigned long Time)
{
  // Guard against zero or negative wait times which locks the processor.
  Time = max(1, Time);

  waitcnt( (Time * (CLKFREQ / Ms)) + CNT );
}

void delayMicroseconds(uint16_t Time)
{
  // Guard against zero or negative wait times which locks the processor.
  Time = max(1, Time);

  waitcnt( microsecondsToClockCycles(Time) + CNT );
}

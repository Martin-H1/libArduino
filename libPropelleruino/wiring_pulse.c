/**
 * @file wiring_pulse.c
 * @brief Provides Arduino types and functions on the Propeller
 *
 * Copyright (c) 2013 by Martin Heermance
 * MIT Licensed
 */

#include <Arduino.h>

static const int Us = 1000000;

static int32_t Pulsin_clk(int32_t Pin, int32_t State)
{
  int32_t Duration = 0;
  DIRA &= ~(1<<Pin);
  CTRA = 0;
  if (State == 1) {
    CTRA = (0x68800000 | Pin);
  } else {
    CTRA = (0x54800000 | Pin);
  }
  FRQA = 1;
  waitpne((State << Pin), ((1<<Pin)));
  PHSA = 0;
  waitpeq((State << Pin), ((1<<Pin)));
  waitpne((State << Pin), ((1<<Pin)));
  Duration = PHSA;
  CTRA = 0;
  return Duration;
}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
  int32_t Duration = 0;
  Duration = (((Pulsin_clk(pin, state) / Us) / 2) + 1);
  return Duration;
}

void pulseOut(uint8_t pin, unsigned long duration)
{
  digitalWrite(pin, HIGH);
  duration = max(duration - 20, 1);
  delayMicroseconds(duration);
  digitalWrite(pin, LOW);
}
/**
 * @file Arduino.cpp
 * @brief Provides Arduino types and functions on the Propeller
 *
 * Copyright (c) 2013 by Martin Heermance
 * MIT Licensed
 */

#include <Arduino.h>

void pinMode(uint8_t pin, uint8_t mode)
{
  DIRA = ((DIRA & (~(1 << pin))) | ((mode & 0x1) << pin));
}

void digitalWrite(uint8_t pin, uint8_t value)
{
  OUTA = ((OUTA & (~(1 << pin))) | ((value & 0x1) << pin));
}

int digitalRead(uint8_t pin)
{
  return ((INA >> pin) & 0x1);
}

/**
 * @file Serial.cpp
 * @brief Provides Arduino Serial funtions on the Propeller
 *
 * Copyright (c) 2013 by Martin Heermance
 * MIT Licensed
 */


#include "Arduino.h"
#include <stdio.h>

int Serial_::available(void)
{
    return false;
}

void Serial_::begin(uint16_t baud_count)
{
}

void Serial_::end(void)
{
}

void Serial_::flush(void)
{
}

void Serial_::print(const char * str)
{
    printf("%s", str);
}

void Serial_::println(long num)
{
    printf("%d\n", num);
}

void Serial_::println(const char * str)
{
    printf("%s\n", str);
}

int Serial_::read(void)
{
    return getchar();
}

Serial_ Serial;
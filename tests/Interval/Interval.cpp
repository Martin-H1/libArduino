/**
 * This is the unit test for the Interval classs.
 */
#include <Arduino.h>
#include <stdio.h>
#include <Interval.h>

long value = 0;

void intervalCallback(void *par)
{
  long * value = (long*)par;
  (*value)++;
}

void setup()
{
  Serial.println("In setup");
  Interval::attach(&intervalCallback, 1000, &value);
}

void loop()
{
  delay(2000);
  Serial.print("In loop, value = ");
  Serial.println(value);
}


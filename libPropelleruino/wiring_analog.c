/**
 * @file wiring_analog.c
 * @brief Provides Arduino types and functions on the Propeller
 *
 * Copyright (c) 2013 by Martin Heermance
 * ASC ADC Code Copyright (c) 2013 Michael Swartzendruber 
 * MIT Licensed
 */

#include <Arduino.h>

static const int Ctrduty = 6;
static const int Dutyperiod = 0x1010101;   

// Assume Propeller ASC for analogRead for now.
// Note the code below is an Arduino-fied version of code given
// to me by Michael Swartzendruber in the Parallax forum.

const char   SE_0  = 0x08;              // ch0  single ended  
const char   SE_1  = 0x09;
const char   SE_2  = 0x0A;
const char   SE_3  = 0x0B;
const char   SE_4  = 0x0C;
const char   SE_5  = 0x0D;
const char   SE_6  = 0x0E;
const char   SE_7  = 0x0F;

const char   D_01  = 0x00;             // 0+/1-  differential 
const char   D_10  = 0x01;             // 1+/0-
const char   D_23  = 0x02;             // 2+/3-
const char   D_32  = 0x03;             // 3+/2- 
const char   D_45  = 0x04;             // 4+/5-
const char   D_54  = 0x05;             // 5+/4-
const char   D_67  = 0x06;             // 6+/7-
const char   D_76  = 0x07;             // 7+/6-  

static const int adcChannel2SE[8] = {SE_0, SE_1, SE_2, SE_3, SE_4, SE_5, SE_6, SE_7};
static const int clockDelay = 400;

// pins for the Propeller ASC A/D convertor.
volatile int  cs;
volatile int  clk;
volatile int  dio;

void init( int cspin, int clkpin, int diopin )
{
  cs  = cspin;
  clk = clkpin;
  dio = diopin;

  pinMode( cspin, OUTPUT );
  digitalWrite( cspin, HIGH );

  pinMode( clkpin, OUTPUT );
  digitalWrite( clkpin, LOW );
}

void finalize()
{
  pinMode( cs, INPUT );
  pinMode( clk, INPUT );
  pinMode( dio, INPUT );
}

int do_adc_cmd( int mux ) 
{
  int level = 0;
  int counter;

  // Read MCP3208 channel
  // -- mux is encoded mux bits for channel/mode

  digitalWrite( cs, LOW );                                      // activate adc
  pinMode( dio, OUTPUT );                                       // dio is output

  // output mux bits, MSBFIRST
  
  for ( counter = 0; counter < 5; counter++ )
  { // send mux bits
    digitalWrite(dio, (mux & 0x10) ? HIGH : LOW);
    delayMicroseconds( clockDelay );
    mux <<= 1;
    digitalWrite( clk, HIGH );                                  // clock the bit
    delayMicroseconds( clockDelay );
    digitalWrite( clk, LOW );
  }

  // input data bits, MSBPOST
  
  pinMode( dio, INPUT );                                        // dio is input

  level = 0;                                                    // clear work var  

  for ( counter = 0; counter < 13; counter++ )
  { // null + 12 data bits
    digitalWrite( clk, HIGH );                                  // clock the bit
    delayMicroseconds( clockDelay );
    digitalWrite( clk, LOW );
    delayMicroseconds( clockDelay );
    level = ( level << 1 ) | digitalRead( dio );                // input data bit
  }

  digitalWrite( cs, HIGH );                                     // de-activate adc
  
  return( level & 0x0fff );
}

// -- ch is channel, 0 to 7
// -- mode is 0 for single-ended, 1 for differential
int read_adc_channel( int ch_mode )
{
  long adc_cmd;

  adc_cmd = 0x10 | ch_mode ;  // create mux bits

  return do_adc_cmd( adc_cmd ); 
}

int analogRead(uint8_t adcChannel)
{
  init( 27, 25, 26 );
  int value = read_adc_channel(adcChannel2SE[adcChannel]);

  value = map(value, 0, 4095, 0, 1023);

  return value;
}

void analogReference(uint8_t mode)
{
}

void analogWrite(uint8_t pin, int duty)
{
  CTRA = 0;
  FRQA = (Dutyperiod * duty);
  CTRA = ((Ctrduty << 26) + pin);
}

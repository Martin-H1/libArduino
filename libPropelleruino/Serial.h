/**
 * @file Serial.h
 * @brief Provides Arduino Serial funtions on the Propeller
 *
 * Copyright (c) 2013 by Martin Heermance
 * MIT Licensed
 */

#ifndef Serial_h
#define Serial_h

class Serial_
{
public:
  int available(void);
  void begin(uint16_t baud_count);
  void end(void);
  void flush(void);
  void print(const char * str);
  void println(long num);
  void println(const char * str);
  int read(void);
};
extern Serial_ Serial;
#endif
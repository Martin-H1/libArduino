/*
  Servo.h - Cog driven Servo library for Propeller chip

  The interface for this library is based upon the Arduino version by Michael Margolis,
  but the implementation is brand new for the port to Parallax Propeller

  Copyright (c) 2013 Martin Heermance.  All right reserved.
*/

/*
  A servo is activated by creating an instance of the Servo class passing the desired pin to the attach() method.
  The servos are pulsed in the background using the value most recently written using the write() method

  The methods are:

   Servo - Class for manipulating servo motors connected to Propeller pins.

   attach(pin, min, max) - Attaches to a pin setting min and max values in microseconds default min
                           is 544, max is 2400. The Propeller version uses default parameters for the one arg call.
 
   write()     - Sets the servo angle in degrees.  (invalid angle that is valid as pulse in microseconds is treated as microseconds)
   writeMicroseconds() - Sets the servo pulse width in microseconds 
   read()      - Gets the last written servo pulse width as an angle between 0 and 180. 
   readMicroseconds()   - Gets the last written servo pulse width in microseconds. (was read_us() in first release)
   attached()  - Returns true if there is a servo attached. 
   detach()    - Stops an attached servos from pulsing its i/o pin.
   setRamp     - smoothly transitions between two positions by stepSize.
 */

#ifndef Servo_h
#define Servo_h

#include <inttypes.h>

#define MAX_SERVOS              8     // I'm rating this lib for eight servos, but it might handle more.
#define INVALID_SERVO         255     // flag indicating an invalid servo index
#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo  
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo 
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached

class Servo
{
public:

  Servo();

  // attach the given pin to a servo, sets pinMode, returns channel number or 0 if failure
  uint8_t attach(int16_t pin, int16_t min = MIN_PULSE_WIDTH, int16_t max = MAX_PULSE_WIDTH);

  void detach();

  void write(int16_t value);         // if value is < 200 its treated as an angle, otherwise as pulse width in microseconds 

  void writeMicroseconds(int16_t value); // Write pulse width in microseconds 

  int16_t read();                    // returns current pulse width as an angle between 0 and 180 degrees

  int16_t readMicroseconds();        // returns current pulse width in microseconds for this servo (was read_us() in first release)

  bool attached();                   // return true if this servo is attached, otherwise false 

  void setRamp(int16_t stepSize);    // Extra feature for the Propeller port. This supports automatic ramping

private:
  class Properties
  {
    public:
      int16_t _pin;
      int16_t _currentPulse;
      int16_t _previousPulse;
      int16_t _stepSize;
    
      Properties() : _pin(-1), _currentPulse(DEFAULT_PULSE_WIDTH), _previousPulse(DEFAULT_PULSE_WIDTH), _stepSize(2000) { }
  };

  static volatile Properties _slots[MAX_SERVOS];
  static volatile int16_t _attachedCount;
  static volatile int16_t _servoCount;
  static volatile unsigned int _servoCog;  // Cog initialazied to zero
  static volatile unsigned int _lockID;    // Lock ID 
  static void servoLoop(void *par);

  uint8_t _idx;                     // index into the array of properties
  int16_t _min;                     // minimum is this value times 4 added to MIN_PULSE_WIDTH    
  int16_t _max;                     // maximum is this value times 4 added to MAX_PULSE_WIDTH
};

#endif

/**
 * TERMS OF USE: MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
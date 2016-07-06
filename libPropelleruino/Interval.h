/*
  Interval.h - Cog driven timer library for Propeller chip

  This class is used to generate periodic callbacks for servo pulsing and time keeping.

  Copyright (c) 2014 Martin Heermance.  All right reserved.
*/

/*
  A consumer calls the attach function with a function callback and minimum interval.

  The methods are:

   Interval - Class callbacks.

   attach(function, interval) - Attaches function to be called at interval in ms
   detach(function) - detaches the function from being called.
 */

#ifndef Interval_h
#define Interval_h

#include <inttypes.h>

#define MAX_SLOTS               8     // I'm rating this lib for eight callbacks, it might handle more.
#define INVALID_CALLBACK      255     // flag indicating an unattached callback.

// Function pointer called IntervalCallback that takes a void pointer
typedef void (*IntervalCallback)(void *par);

class Interval
{
public:

  // attaches the function and calls it with the blob
  static void attach(IntervalCallback callback, uint16_t interval, void * blob);

  // terminates calls to callback
  static void detach(IntervalCallback callback);

private:
  class Slot
  {
    public:
      IntervalCallback _callback;
      uint16_t         _interval;
      uint32_t         _lastTime;
      void *           _blob;
    
      Slot() { detach(); }

      void attach(IntervalCallback callback, uint32_t interval, void * blob)
      {
        _callback = callback;
        _interval = interval * (CLKFREQ / 1000);
        _lastTime = CNT;
        _blob = blob;
      }

      void detach()
      {
        _callback = NULL;
        _interval = -1;
        _lastTime = 0;
        _blob = NULL;
      }

      void invokeCallback()
      {
        _lastTime = CNT;
        _callback(_blob);
      }

      boolean isAttached() { return _callback != NULL; }

      boolean isDue();
  };

  static Slot _slots[MAX_SLOTS];
  static volatile unsigned int _intervalCog;  // Cog initialazied to zero
  static volatile unsigned int _lockID;       // Lock ID 
  static void intervalLoop(void *par);
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
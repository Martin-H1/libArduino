/**
 * @file servo.cpp
 *
 * This file is a mashup of the Propeller IDE servo library implementation with the
 * Arudino interface.
 *
 * @author Martin Heermance
 */

#include <Arduino.h>
#include <stdio.h>
#include "Servo.h"

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->_min * 4)  // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->_max * 4)  // maximum value in uS for this servo 

// For LMM cog 160 bytes declared as int
// Add enough for stack & calcs (50 bytes declared as int 
unsigned int stack[(160 + (90 * 4)) / 4];

volatile Servo::Properties Servo::_slots[MAX_SERVOS];
volatile int16_t Servo::_attachedCount = 0;
volatile int16_t Servo::_servoCount = 0;
volatile unsigned int Servo::_servoCog = 0;
volatile unsigned int Servo::_lockID = locknew();

/* Constructor which creates one lock on first construction.
 * This is not completely thread safe, but these are created
 * during program setup before backgroup processing starts.
 */
Servo::Servo()
{
  // Now we're thread safe, so set the lock.
  while(lockset(Servo::_lockID));

  // if we still have free slots.
  if( Servo::_servoCount < MAX_SERVOS)
  {
    // Grab a slot in the properties array.
    _idx = Servo::_servoCount;

    // Increment the servo count.
    Servo::_servoCount++;
  }
  else
  {
    // too many servos 
    _idx = INVALID_SERVO;
  }

  // Clear lock
  lockclr(Servo::_lockID);
}

/* Binds a pin to this servo object and sets the limits.
 */
uint8_t Servo::attach(int16_t pin, int16_t min, int16_t max)
{
  // capture the config for future write methods.
  _min  = (MIN_PULSE_WIDTH - min)/4; //resolution of min/max is 4 uS
  _max  = (MAX_PULSE_WIDTH - max)/4; 

  // Set lock before touching the shared properties array
  while(lockset(Servo::_lockID));

  // If cog not started, start it.
  if (Servo::_servoCog == 0)
  {
    Servo::_servoCog = cogstart(&servoLoop, NULL, stack, sizeof(stack)) + 1;
  }

  Servo::_slots[_idx]._pin = pin;
  pinMode(Servo::_slots[_idx]._pin, OUTPUT);

  // Increment when a servo is attached.
  Servo::_attachedCount++;

  // Clear lock
  lockclr(Servo::_lockID);
}

/* Releases a pin from this servo object and resets the slot state.
 */
void Servo::detach()
{
  // Set lock.
  while(lockset(Servo::_lockID));

  Servo::_slots[_idx]._pin = -1;
  Servo::_slots[_idx]._currentPulse = DEFAULT_PULSE_WIDTH;
  Servo::_slots[_idx]._previousPulse = DEFAULT_PULSE_WIDTH;
  Servo::_slots[_idx]._stepSize = 2000;

  // Decrement the attached servo count.
  Servo::_attachedCount--;

  // if no more attached servos and the cog is running,
  // then stop the cog and record that it's stopped.
  if (Servo::_attachedCount == 0 && Servo::_servoCog != 0)
  {
    cogstop(Servo::_servoCog-1);
    Servo::_servoCog = 0;
  }

  // Clear lock
  lockclr(Servo::_lockID);
}

void Servo::write(int16_t value)
{
  // treat values less than min pulse width as angles in degrees
  if (value < MIN_PULSE_WIDTH)
  {
    if (value < 0)
      value = 0;
    if(value > 180)
      value = 180;

    value = map(value, 0, 180, SERVO_MIN(),  SERVO_MAX());
  }
  writeMicroseconds(value);
}

void Servo::writeMicroseconds(int16_t value)
{
  // Set the lock
  while(lockset(Servo::_lockID));

  Servo::_slots[_idx]._currentPulse = value;

  // Clear the lock before return.
  lockclr(Servo::_lockID);
}

/* Returns the current pulse value in microseconds, note that if ramping
 * is in use this value might not be the value set via write.
 */
int16_t Servo::read() // return the value as degrees
{
  return map( this->readMicroseconds() + 1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

/* Returns the current pulse value in microseconds, note that if ramping
 * is in use this value might not be the value being sent to the servo.
 */
int16_t Servo::readMicroseconds()
{
  int retVal = -1;

  // Set the lock
  while(lockset(Servo::_lockID));

  // capture the current pulse value.
  retVal = Servo::_slots[_idx]._currentPulse;

  lockclr(Servo::_lockID);

  return retVal;
}

/* Returns true if the pin has a value.
 */
bool Servo::attached()
{
  bool retVal;

  // Set the lock
  while(lockset(Servo::_lockID));

  // capture the current pulse value.
  retVal = Servo::_slots[_idx]._pin != -1 ? true : false;

  lockclr(Servo::_lockID);

  return retVal;
}

/* Sets the ramp rate.
 */
void Servo::setRamp(int16_t stepSize)
{
  // Set the lock
  while(lockset(Servo::_lockID));

  Servo::_slots[_idx]._stepSize = stepSize;

  // Clear the lock before return.
  lockclr(Servo::_lockID);
}

/* servo tending cog method.
 */
void Servo::servoLoop(void *par)
{
  // Servo control loop runs until stopped.
  while(true)                                 
  {
    // For each servo.
    for(uint8_t i = 0; i < MAX_SERVOS; i++)
    {
      // Set the lock
      while(lockset(Servo::_lockID));

      // If this servo slot is bound to a pin.
      if (Servo::_slots[i]._pin != -1)
      {

        // Copy requested position to var
        int16_t tPulse = Servo::_slots[i]._currentPulse;

        // Compute the required size of change.
        int16_t diff = tPulse - Servo::_slots[i]._previousPulse;

        // If change larger than ramp step
        if (Servo::_slots[i]._stepSize < abs(diff))                          
        {
          int16_t step = Servo::_slots[i]._stepSize;
          // If the difference is negative, then make the step negative.
          if (diff < 0)
          {
            step = -step;
          }

          // Increment pulse by step to get closer to target.
          tPulse = Servo::_slots[i]._previousPulse + step;              
        }

        // Send pulse to servo and remember pulse for next time
        pinMode(Servo::_slots[i]._pin, OUTPUT);
        pulseOut(Servo::_slots[i]._pin, tPulse);
        Servo::_slots[i]._previousPulse = tPulse;
      }

      // Clear the lock.
      lockclr(Servo::_lockID);
    }

    // Sleep until next pulse required.
    delay(20);
  }
}
/**
 * @file Interval.cpp
 *
 * This file is the implementation of 
 *
 * @author Martin Heermance
 */

#include <Arduino.h>
#include <stdio.h>
#include <limits.h>
#include "Interval.h"

// For LMM cog 160 bytes declared as int
// Add enough for stack & calcs (50 bytes declared as int 
unsigned int stack[(160 + (90 * 4)) / 4];

Interval::Slot Interval::_slots[MAX_SLOTS];
volatile unsigned int Interval::_intervalCog = 0;
volatile unsigned int Interval::_lockID = locknew();

/* Binds a function to an interval
 */
void Interval::attach(IntervalCallback callback, uint16_t interval, void * blob)
{
  // Set lock before touching the shared properties array
  while(lockset(Interval::_lockID));

  // If cog not started, start it.
  if (Interval::_intervalCog == 0)
  {
    Interval::_intervalCog = cogstart(&intervalLoop, NULL, stack, sizeof(stack)) + 1;
  }

  // find a free Interval slot.
  for(uint8_t i = 0; i < MAX_SLOTS; i++)
  {
    if (!Interval::_slots[i].isAttached())
      Interval::_slots[i].attach(callback, interval, blob);
  }

  // Clear lock
  lockclr(Interval::_lockID);
}

/* Releases a pin from this Interval object and resets the slot state.
 */
void Interval::detach(IntervalCallback callback)
{
  // Set lock.
  while(lockset(Interval::_lockID));

  int attachedCount = 0;

  // find the Interval slot.
  for(uint8_t i = 0; i < MAX_SLOTS; i++)
  {
    if (Interval::_slots[i]._callback == callback)
      Interval::_slots[i].detach();

    if (Interval::_slots[i].isAttached())
      attachedCount++;
  }

  // if no more attached Slot and the cog is running,
  // then stop the cog and record that it's stopped.
  if (attachedCount == 0 && Interval::_intervalCog != 0)
  {
    cogstop(Interval::_intervalCog-1);
    Interval::_intervalCog = 0;
  }

  // Clear lock
  lockclr(Interval::_lockID);
}

/* Interval tending cog method.
 */
void Interval::intervalLoop(void *par)
{
  // Interval control loop runs until stopped.
  while(true)                                 
  {
    // For each Interval.
    for(uint8_t i = 0; i < MAX_SLOTS; i++)
    {
      // Set the lock
      while(lockset(Interval::_lockID));

      // If this Interval slot is bound to a callback
      if (Interval::_slots[i].isAttached() && Interval::_slots[i].isDue())
      {
        Interval::_slots[i].invokeCallback();
      }

      // Clear the lock.
      lockclr(Interval::_lockID);
    }
  }
}

boolean Interval::Slot::isDue()
{
  uint32_t now = CNT;

  // if the counter hasn't wrapped then the math is simple.
  if (now > _lastTime)
  {
    if (now > (_lastTime + _interval))
      return true;
  }
  else
  {
    if ((ULONG_MAX - _lastTime + now) > _interval)
      return true;
  }

  return true;
}
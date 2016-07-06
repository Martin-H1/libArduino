// Sweep
// by BARRAGAN <http://barraganstudio.com> 
// This example code is in the public domain.

#include <Arduino.h>
#include <stdio.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created

Servo myservo2;  // create servo object to control a servo
                // a maximum of eight servo objects can be created

int pos = 0;    // variable to store the servo position

void setup()
{
  delay(1000);
  printf("In setup about to attach servo.\n");
  myservo.attach(12);  // attaches the servo on pin 16 to the servo object
  myservo2.attach(13);  // attaches the servo on pin 16 to the servo object
}

void loop()
{
  printf("\nin loop about to sweep servo up.\n");

  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees
  {                                  // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
    printf("pos = %d.\n", myservo.read());
  }

  printf("\nin loop about to sweep servo down.\n");
  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees
  {
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    myservo2.write(pos);              // tell servo to go to position in variable 'pos'
    delay(20);                       // waits 15ms for the servo to reach the position
    printf("pos = %d.\n", myservo.read());
  }
}
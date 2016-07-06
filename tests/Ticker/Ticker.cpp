/**
* This is the main Blank Simple C++ Project program file.
*/

#include <Arduino.h>
#include <stdio.h>
#include "Adafruit_GFX.h"
#include "Max72xxPanel.h"

int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 1;

Max72xxPanel matrix = Max72xxPanel(0, 1, 2, numberOfHorizontalDisplays, numberOfVerticalDisplays);

char tape[] = "Propelleruino";
int wait = 200; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

void setup() {
  // start serial port at 9600 bps and wait for port to open:
 // Serial.begin(9600);

  matrix.setIntensity(7); // Use a value between 0 and 15 for brightness
}

void loop() {

  for ( int i = 0 ; i < width * strlen(tape) + matrix.width() - 1 - spacer; i++ ) {

    matrix.doubleBuffering(true); // Prevent screen flicker

    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < strlen(tape) ) {
        matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
      }

      letter--;
      x -= width;
    }

    matrix.doubleBuffering(false); // Send drawing to display

    delay(wait);
  }
}

/******************************************************************
 A library for controling a set of 8x8 LEDs with a MAX7219 or MAX7221.
 This is a plugin for Adafruit's core graphics library, providing
 basic graphics primitives (points, lines, circles, etc.).
 You need to download and install Adafruit_GFX to use this library.

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source hardware
 by purchasing products from Adafruit!

 Written by Mark Ruys.
 BSD license, check license.txt for more information.
 All text above must be included in any redistribution.

 Datasheet: http://datasheets.maximintegrated.com/en/ds/MAX7219-MAX7221.pdf

 ******************************************************************/

#ifndef Max72xxPanel_h
#define Max72xxPanel_h

#include <Arduino.h>
#include <SPI.h>

class Adafruit_GFX;

class Max72xxPanel : public Adafruit_GFX {

public:

  /*
   * Create a new controler
   * Params :
   * clkPin		pin for clocking the output
   * csPin		pin for selecting the device
   * sdaPin		pin for sending data to the device
   * hDisplays  number of displays horizontally
   * vDisplays  number of displays vertically
   */
  Max72xxPanel(int clkPin, int csPin, int sdaPin, int hDisplays=1, int vDisplays=1);

  void drawPixel(int16_t x, int16_t y, uint16_t color);
  void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
  void fillScreen(uint16_t color);

  /*
   * Set the shutdown (power saving) mode for the device
   * Params :
   * status	If true the device goes into power-down mode. Set to false
   *		for normal operation.
   */
  void shutdown(boolean status);

  /*
   * Set the brightness of the display.
   * Params:
   * intensity	the brightness of the display. (0..15)
   */
  void setIntensity(int intensity);

  /*
   * To prevent flicker, enable double buffering while drawing.
   * When you disable double buffering, your drawing is copied
   * to the display(s).
   * Params :   If true double buffering is enabled
   */
  void doubleBuffering(boolean enabled);

private:
  SPI _spi;

  uint8_t SCLK; // serial clock (output from master)
  uint8_t MOSI; // master output, slave input (output from master)
  uint8_t SS;   // slave select (active low, output from master)

  /* Send out a single command to the device */
  void spiTransfer(byte opcode, byte data=0);
  void drawLineHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);

  /* We keep track of the led-status for all max 8 devices in this array */
  byte *buffer;
  boolean doubleBuffer;
  int bufferSize;
};

#endif	// Max72xxPanel_h

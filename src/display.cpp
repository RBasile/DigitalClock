/*
  Copyright 2022 Basile Rouault
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
  to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute and/or sublicense,
  copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <Arduino.h>
#include "globals.h"
#include "display.h"

uint16_t Hue = 10;
uint8_t Sat = 255;
uint8_t Lum = 100;

uint8_t gardNum = 1;


const uint8_t Gradold[3][LED_COUNT]={
{
   1, 3, 4, 3, 1, 0, 2,
   6, 8, 9, 8, 6, 5, 7,
  11,13,14,13,11,10,12,
  16,18,19,18,16,15,17,
  9,10,10
},
{
   0, 2, 5, 6, 4, 1, 3,
   3, 5, 7, 8, 6, 4, 5,
   5, 7,10,11, 8, 6, 8,
   7,10,12,12,11, 8,10,
   5,8,9
},
{
   7,5,5,7,8,8,6,
   3,1,1,3,4,4,2,
   3,4,4,3,1,1,2,
   7,8,8,7,5,5,6,
   0,0,2
}
};

const char *characterMap[20] = {
      "1111110", "0110000", "1101101", "1111001", //0 1 2 3
      "0110011", "1011011", "1011111",  //4 5 6
      "1110000", "1111111", "1111011",  //7 8 9
      "0000000", 
      "1000000", "0100000", "0010000", "0001000", "0000100", "0000010", "0000001",
      "1110111", "1001110" // A(18) C(19)
      };

const uint8_t Grad[3][LED_COUNT]={
{
13,40,54,40,13,0,27,
81,107,121,107,81,67,94,
148,174,188,174,148,134,161,
215,242,255,242,215,201,228,
121,134,134
},
{
0,43,106,128,85,21,64,
64,106,149,170,128,85,106,
106,149,213,234,170,128,170,
149,213,255,255,234,170,213,
106,170,191
},
{
223,159,159,223,254,254,191,
95,32,32,95,127,127,64,
95,127,127,95,32,32,64,
223,254,254,223,159,159,191,
0,0,64
}
};

void colorWipe(uint32_t color, int wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void separateur(bool one,bool too,bool three){
  if (one){
    strip.setPixelColor(28,strip.gamma32(strip.ColorHSV((Hue+deoTime)+Grad[gardNum][28]*scaleH)));
  }
  else{strip.setPixelColor(28,0,0,0);}
  if (too){
    strip.setPixelColor(29,strip.gamma32(strip.ColorHSV((Hue+deoTime)+Grad[gardNum][29]*scaleH)));
  }
  else{strip.setPixelColor(29,0,0,0);}
  
  if (three){
    strip.setPixelColor(30,strip.gamma32(strip.ColorHSV((Hue+deoTime)+Grad[gardNum][30]*scaleH)));
  }
  else{strip.setPixelColor(30,0,0,0);}
}

void DigitDisp(byte digNum, byte number){
  String  bitmask = characterMap[number];
  int charPos = 0;
  // expand bitbask to number of pixels per segment in the proper position
  for (int x = (digNum) * 7; x <= (digNum) * 7 + 6; x++) {
    int pixelHue = (Hue+deoTime)+Grad[gardNum][x]*scaleH;
    if (bitmask.charAt(charPos) == '1') {
      // Lighting up
      strip.setPixelColor(x,strip.gamma32(strip.ColorHSV(pixelHue)));                    
    } else {
      // Turning off
      strip.setPixelColor(x,0,0,0);                    
    }
    charPos++;
  }
}
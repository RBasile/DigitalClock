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
#include <OneWire.h>
#include <DallasTemperature.h>
#include "globals.h"
#include "display.h"

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensorsT(&oneWire);


void setupTemp(void){
  // Start up the library
  sensorsT.begin();
  sensorsT.setWaitForConversion(false);
}
void displayTemp(void){
  float temperature;
  if (turn == 5){
    sensorsT.requestTemperatures(); // Send the command to get temperatures
    }
  if (turn == 0){
  temperature = sensorsT.getTempCByIndex(0);
  //Serial.println(temperature);
  }
  DigitDisp(0,int(temperature)/10 % 10);
  DigitDisp(1,int(temperature) % 10);
  DigitDisp(2,int(temperature*10) % 10);
  //DigitDisp(3,int(temperature*100) % 10);
  DigitDisp(3,19);  // display C
  separateur(0,0,1);
  deoTime+=10;
  strip.show();
}
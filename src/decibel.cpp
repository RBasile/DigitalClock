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
#include "decibel.h"
#include "display.h"


int dbcalc(){  
  int dBcal;
  long sum = 0 ; // Store the total value of n measurements  
  ADMUX = 0x01;
  for ( int i = 0 ; i <num_Measure; i ++)  
  {  
    while (!(ADCSRA & /*0x10*/_BV(ADIF))); // wait for adc to be ready (ADIF)
        sbi(ADCSRA, ADIF); // restart adc
        byte m = ADCL; // fetch adc data
        byte j = ADCH;
        int k = ((int)j << 8) | m; // form into an int
   int amp = abs(k - 512);
   sum += ((long)amp*amp);
  }
  
  float level = sqrt(sum/num_Measure);
  float dBdiy = 20.0*log10(level/0.01024)*1.6-50;  //*1.6-50 coretion based on smarthPhone DB mesure
  //Serial.print(level);
  //Serial.print(" ");
  //Serial.println(dBdiy);

  return dBcal = (int) dBdiy;//*1.15-4.18;
  //Serial.print(dBdiy);
  //Serial.print("  ");
  //Serial.println(dBcal);
}
void sentSoundsample(){  
  uint16_t kata[512];
  ADMUX = 0x01;
  TCNT1 = 0;
  for ( int i = 0 ; i <512; i ++)  
  {  
    while (!(ADCSRA & /*0x10*/_BV(ADIF))); // wait for adc to be ready (ADIF)
        sbi(ADCSRA, ADIF); // restart adc
        byte m = ADCL; // fetch adc data
        byte j = ADCH;
        kata[i] = ((int)j << 8) | m; // form into an int
  }
  //Serial.println((float)TCNT1/15625,6);
  for ( int i = 0 ; i <512; i ++)  
  {  
    Serial.println(((float)kata[i]-514)/256,5);
  }
}

void setupDB(){
analogReference(EXTERNAL); // 3.3V to AREF
  // set the adc to free running mode
  // register explanation: http://maxembedded.com/2011/06/the-adc-of-the-avr/
  // 5 => div 32. sample rate 38.4
  // 7 => switch to divider=128, default 9.6khz sampling
  ADCSRA = 0xe0+7; // "ADC Enable", "ADC Start Conversion", "ADC Auto Trigger Enable" and divider.
  ADMUX = 0x00; // use adc0 (hardcoded, doesn't use MicPin). Use ARef pin for analog reference (same as analogReference(EXTERNAL)).
  DIDR0 = 0x01; // turn off the digital input for adc0
  // change ADC freq divider. default is div 128 9.6khz (bits 111)
  // http://yaab-arduino.blogspot.co.il/2015/02/fast-sampling-from-analog-input.html
  // 1 0 0 = mode 4 = divider 16 = 76.8khz
  //sbi(ADCSRA, ADPS2);
  //cbi(ADCSRA, ADPS1);
  //cbi(ADCSRA, ADPS0);
  // 1 0 1 = mode 5 = divider 32 = 38.4Khz
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);
}
  

void dbDisplay(){
  int DBval = dbcalc();
  DigitDisp(0,DBval/100 % 10);
  DigitDisp(1,DBval/10 % 10);
  DigitDisp(2,DBval % 10);
  DigitDisp(3,17);  // display C
  deoTime+=10;
  strip.show();
}
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
#include "arduinoFFT.h"
#include "globals.h"
#include "display.h"

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*
These values can be changed in order to evaluate the functions
*/
const uint16_t samples = 64; //This value MUST ALWAYS be a power of 2
const double samplingFrequency = 38000;
/*
These are the input and output vectors
Input vectors receive computed results from FFT
*/
bool fftmoy[5];
byte fftmoyTotal;


bool whistlingLow(double *vReal){
  double target = vReal[2];
  
  if (target>60){
    if (vReal[1]>target*0.5){
      return false;
    }
    if (vReal[3]>target*0.6){
      return false;
    }
    if (vReal[4]>target*0.2){
      return false;
    }
    if (vReal[5]>target*0.2){
      return false;
    }
    if (vReal[6]>target*0.2){
      return false;
    }
    if (vReal[7]>target*0.2){
      return false;
    }
    if (vReal[8]>target*0.2){
      return false;
    }
    double limit = target*0.15;
    for (uint16_t i = 9; i < (samples >> 1); i++){
      if (vReal[i]>limit){
        return false;
      }
    }
  return true;
  }
  return false;
} 

bool whistlingHigh(double *vReal){
  double target = vReal[3];
  
  if (target>60){
    if (vReal[2]>target*0.6){
      return false;
    }
    if (vReal[4]>target*0.5){
      return false;
    }
    if (vReal[5]>target*0.2){
      return false;
    }
    if (vReal[6]>target*0.2){
      return false;
    }
    if (vReal[7]>target*0.2){
      return false;
    }
    if (vReal[8]>target*0.2){
      return false;
    }
    if (vReal[9]>target*0.2){
      return false;
    }
    if (vReal[10]>target*0.2){
      return false;
    }
    if (vReal[11]>target*0.2){
      return false;
    }
    double limit = target*0.15;
    for (uint16_t i = 12; i < (samples >> 1); i++){
      if (vReal[i]>limit){
        return false;
      }
    }
  return true;
  }
  return false;
} 

byte fft()
{
  double vReal[samples];
  double vImag[samples];
  ADMUX = 0x01;
  for (uint16_t i = 0; i < samples; i++)
  {  
    while (!(ADCSRA & /*0x10*/_BV(ADIF))); // wait for adc to be ready (ADIF)
        sbi(ADCSRA, ADIF); // restart adc
        byte m = ADCL; // fetch adc data
        byte j = ADCH;
        int k = ((int)j << 8) | m; // form into an int
   vReal[i] = k - 512;
   vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  }

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */


  bool whisthigh = whistlingHigh(vReal);
  bool whistlow = whistlingLow(vReal);

  fftmoyTotal -= fftmoy[turn>>1];
  fftmoy[turn>>1] = whistlow;
  fftmoyTotal += fftmoy[turn>>1];

  //Serial.print(whisthigh);Serial.print(" ");
  //Serial.print(whistlow);Serial.print(" ");
  //Serial.println(fftmoyTotal);

  if (whisthigh){
    deoTime-=250;
  }
    
  if (fftmoyTotal>=3){
    fftmoyTotal = 0;
    fftmoy[0] = 0;fftmoy[1] = 0;fftmoy[2] = 0;fftmoy[3] = 0;fftmoy[4] = 0;
    return 1;
  }
  /*
  if (high && low){
    return 0;
  }
  else if(low){
    return 0;
  }
  else if(high){
    return 2;
  }*/
  return 0;
}
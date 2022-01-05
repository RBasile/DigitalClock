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
#include "RTClib.h"
#include "globals.h"
#include "display.h"

RTC_DS3231 rtc;



//TIME
byte hour = 21;
byte minute = 23;
byte second = 0;
bool dotSec = 1;


void setuptime() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void Settime(){
  scaleH += 140;
  colorWipe(strip.Color(0, 255,0), 20);
  colorWipe(strip.Color(0, 0,0),0);
  
  DateTime now = rtc.now();
  hour = now.hour();
  minute= now.minute();
//set hour
  while (digitalRead(but4Pin)){
    int button2State = digitalRead(but2Pin);
    int button3State = digitalRead(but3Pin);

    if (button3State == LOW){
      hour -= 1;
      if (hour>24){hour = 23;}
      delay(50);
    }
    if (button2State == LOW){
      hour += 1;
      if (hour>23){hour = 0;}
      delay(50);
    }
    DigitDisp(0,hour/10 % 10);
    DigitDisp(1,hour % 10);
    DigitDisp(2,(minute/10) % 10);
    DigitDisp(3,minute % 10);
    delay(100);
    separateur(dotSec,dotSec,0);
    dotSec = not dotSec;
    deoTime -=100;
    strip.show();
  }

  colorWipe(strip.Color(255, 255,0), 20);
  colorWipe(strip.Color(0, 0,0),0);
//set minute
  while (digitalRead(but4Pin)){
    int button2State = digitalRead(but2Pin);
    int button3State = digitalRead(but3Pin);

    if (button3State == LOW){
      minute -= 1;
      if (minute>60){minute = 59;}
      delay(50);
    }
    if (button2State == LOW){
      minute += 1;
      if (minute>59){minute = 0;}
      delay(50);
    }
    DigitDisp(0,hour/10 % 10);
    DigitDisp(1,hour % 10);
    DigitDisp(2,(minute/10) % 10);
    DigitDisp(3,minute % 10);
    delay(100);
    separateur(dotSec,dotSec,0);
    dotSec = not dotSec;
    deoTime -=100;
    strip.show();
  }

  
  now = rtc.now();
  rtc.adjust(DateTime(now.year(), now.month(), now.day(), hour, minute, now.second()));
  
  scaleH -= 140;
}


void Clock(){
  if (turn == 0){
    DateTime now = rtc.now();
    hour = now.hour();
    minute= now.minute();
    dotSec = not dotSec;
  }
  DigitDisp(0,hour/10 % 10);
  DigitDisp(1,hour % 10);
  DigitDisp(2,(minute/10) % 10);
  DigitDisp(3,minute % 10);
  separateur(dotSec,dotSec,0);
  deoTime+=10;
  strip.show();
}

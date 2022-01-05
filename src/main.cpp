#include <Arduino.h>

#include "globals.h"
#include "display.h"

#include "time.h"
#include "temperature.h"
#include "decibel.h"
#include "fft.h"

//globals
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
byte turn = 0;
const int but1Pin = 11;
const int but2Pin = 10;
const int but3Pin = 9;
const int but4Pin = 8;
int scaleH = -40;
uint16_t deoTime = 0;




int buttonTurn = 0;

byte bright[10];
long brightTotal;
bool displayOn = true;


bool lumino(){
  bool Rstate =  false;
  uint16_t sensorValue;
  
  ADMUX = 0x00;
  while (!(ADCSRA & /*0x10*/_BV(ADIF))); // wait for adc to be ready (ADIF)
  sbi(ADCSRA, ADIF); // restart adc
  byte m = ADCL; // fetch adc data
  byte j = ADCH;
  sensorValue = ((int)j << 8) | m; // form Analog input into an int
  
  // Smouthing the lum control
  brightTotal -= bright[turn];
  bright[turn] = map(sensorValue, 600, 1023, 255, 2);
  brightTotal += bright[turn];
  
  byte currentbright = brightTotal/10;
  
  if (displayOn){strip.setBrightness(currentbright);}
  else{strip.setBrightness(0);}

/*  Detect the hand movement that change the luminosity to change the display mode
  uint16_t oldVal;
  uint16_t oldVal2;
  byte detectState = 0;s

  int valdiff = sensorValue-oldVal2;
  if (detectState==0){
    if (valdiff>6){
      detectState = 2;
      }
  }else{
    if (valdiff<-6){
      detectState = 0;
      Rstate = true;
      //colorWipe(strip.Color(50, 0,255), 20);
      //colorWipe(strip.Color(0, 0,0),00);
    }else{
      detectState = detectState - 1;
    }
   
  }
  
  oldVal2 = oldVal;
  oldVal = sensorValue;
  
  //uint16_t durr = TCNT1;
  //TCNT1 = 0; // chronormetre
  //Serial.println(durr);
*/  
  return Rstate;
}

bool button() {
  bool Rstate = false;
  // read the state of the pushbutton value:
  int buttonState = digitalRead(but1Pin);
  int button4State = digitalRead(but4Pin);
  
  if (button4State == LOW){
    buttonTurn += 1;
    if (buttonTurn > 10){
      Settime();
      buttonTurn = 0;
    }
  }
  else{buttonTurn = 0;}
  
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    // turn LED on:
    Rstate = true;
    colorWipe(strip.Color(0, 0,0),20);
  } else {
    // turn LED off:
    //Rstate = false;
  }
  
  byte fftV = fft();
  if (fftV == 1){displayOn=1;Rstate=  true;colorWipe(strip.Color(0, 0,0),20);}
  /*
  else if (fftV == 2){
    colorWipe(strip.Color(255, 0,0),20);
    if (displayOn){displayOn=0;}
    else {displayOn=1;}
   }*/
  return Rstate;
}

void setup() {
  Serial.begin(115200);
        //button
  pinMode(but1Pin, INPUT);
  digitalWrite(but1Pin, HIGH);
  pinMode(but2Pin, INPUT);
  digitalWrite(but2Pin, HIGH);
  pinMode(but3Pin, INPUT);
  digitalWrite(but3Pin, HIGH);
  pinMode(but4Pin, INPUT);
  digitalWrite(but4Pin, HIGH);
  analogReference(EXTERNAL);
  //Timer1
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TCCR1B |= (1 << CS10);    // 1024 prescaler  15625 pour une seconde 
  //read TCNT1 register for the value of Timer1

  setuptime();
  setupTemp();
  setupDB();
  
  // put your setup code here, to run once:
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(255); 
  colorWipe(strip.Color(255, 255,0), 20);
  colorWipe(strip.Color(0, 0,0),0);
  pinMode(LED_BUILTIN, OUTPUT);
  
}


void loop() { // Main LOOP
  while(1){
    dbDisplay();
    ADMUX = 0x00;
    delay(40);
    lumino();
    ADMUX = 0x01;
    delay(40);
    //Serial.println((float)TCNT1/15625,6);
    //TCNT1 = 0;
    turn +=1;
    if (turn >=10){turn = 0;}
    if( button() ){break;}
  }
  
  while(1){
    displayTemp();
    //colorWipe(strip.Color(50, 0,255), 20);
    //colorWipe(strip.Color(255, 0,50),20);
    ADMUX = 0x00;
    delay(40);
    lumino();
    ADMUX = 0x01;
    delay(40);
    turn +=1;
    if (turn >=10){turn= 0;}
    if( button() ){break;}
  }
  turn = 0;
  while(1){
    Clock();
    ADMUX = 0x00;
    delay(37);
    lumino();
    ADMUX = 0x01;
    delay(37);
    turn +=1;
    if (turn >=10){turn = 0;}
    if( button() ){break;}
  }
  turn = 0;
}
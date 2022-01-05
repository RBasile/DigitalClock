#ifndef _display_
#define _display_
#include <Arduino.h>

void colorWipe(uint32_t color, int wait);
void separateur(bool one,bool too,bool three);
void DigitDisp(byte digNum, byte number);

#endif // _display_
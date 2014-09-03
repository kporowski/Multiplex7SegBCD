/*
  Multiplex7SegBCD is a library that allows you to multiplex 1, 2 ,3 or 4 
  7-segment digits using BCD decoder or direct arduino pins connection.

  Copyright (C) 2014  Krzysztof Porowski

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/
#include "Multiplex7SegBCD.h"

byte SEGMENT[16][7] = {
    {1,1,1,1,1,1,0},
    {0,1,1,0,0,0,0},
    {1,1,0,1,1,0,1},
    {1,1,1,1,0,0,1},
    {0,1,1,0,0,1,1},
    {1,0,1,1,0,1,1},
    {1,0,1,1,1,1,1},
    {1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1},
    {1,1,1,1,0,1,1},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0},
};

byte BCD[16][4] = {
    {0,0,0,0},
    {1,0,0,0},
    {0,1,0,0},
    {1,1,0,0},
    {0,0,1,0},
    {1,0,1,0},
    {0,1,1,0},
    {1,1,1,0},
    {0,0,0,1},
    {1,0,0,1},
    {0,1,0,1},
    {1,1,0,1},
    {0,0,1,1},
    {1,0,1,1},
    {0,1,1,1},
    {1,1,1,1}
};

Multiplex7SegBCD::Multiplex7SegBCD(byte noOfDigits, byte *digitPins, byte *bcd4Pins, byte segmentSelectedState)
{
  lastUpdate = millis();
  msecs = 1;
  byte i;
  for (i = 0; i < noOfDigits; i++)
    pinMode(digitPins[i], OUTPUT);
  for (i = 0; i < 4; i++)
    pinMode(bcd4Pins[i], OUTPUT); 
  digitsNo = noOfDigits; 
  digitsPtr = digits;
  digitPinsPtr = digitPins;
  segmentPinsPtr = bcd4Pins;
  bcdPinsPresent = 1; // uses BCD pins only
  selectedState = segmentSelectedState;
  if (selectedState == HIGH)
    unselectedState = LOW;
  else
    unselectedState = HIGH;
  digitValueCurrent = 0;
  digitValuePrevious = 0;
}

Multiplex7SegBCD::Multiplex7SegBCD(byte noOfDigits, byte *digitPins, byte *segmentPins, byte *bcd4Pins, byte segmentSelectedState)
{
  lastUpdate = millis();
  msecs = 1;
  byte i;
  for (i = 0; i < noOfDigits; i++)
    pinMode(digitPins[i], OUTPUT);
  if (bcd4Pins != NULL)
  {
    for (i = 0; i < 4; i++)
      pinMode(bcd4Pins[i], OUTPUT); 
    bcdPinsPresent = 1; // uses BCD pins
    segmentPinsPtr = bcd4Pins;
  }
  if (segmentPins != NULL)
  {
    for (i = 0; i < 7; i++)
      pinMode(segmentPins[i], OUTPUT); 
    bcdPinsPresent = 0; // uses SEGMENT pins
    segmentPinsPtr = segmentPins;
  }
  digitValueCurrent = 0;
  digitValuePrevious = 0;
  digitsNo = noOfDigits; 
  digitsPtr = digits;
  digitPinsPtr = digitPins;
  selectedState = segmentSelectedState;
  if (selectedState == HIGH)
    unselectedState = LOW;
  else
    unselectedState = HIGH;
}

void Multiplex7SegBCD::tick()
{
  unsigned long current_millis = millis();
  if (current_millis - lastUpdate >= msecs) 
  {
    lastUpdate = current_millis;

    digitalWrite(*(digitPinsPtr + digitValuePrevious), unselectedState);
    byte digit = *(digitsPtr + digitValueCurrent);
    if (bcdPinsPresent != 1)
    {
      digitalWrite(*segmentPinsPtr, SEGMENT[digit][0]);
      digitalWrite(*(segmentPinsPtr + 1), SEGMENT[digit][1]);
      digitalWrite(*(segmentPinsPtr + 2), SEGMENT[digit][2]);
      digitalWrite(*(segmentPinsPtr + 3), SEGMENT[digit][3]);
      digitalWrite(*(segmentPinsPtr + 4), SEGMENT[digit][4]);
      digitalWrite(*(segmentPinsPtr + 5), SEGMENT[digit][5]);
      digitalWrite(*(segmentPinsPtr + 6), SEGMENT[digit][6]);
    }
    else
    {
      digitalWrite(*segmentPinsPtr, BCD[digit][0]);
      digitalWrite(*(segmentPinsPtr + 1), BCD[digit][1]);
      digitalWrite(*(segmentPinsPtr + 2), BCD[digit][2]);
      digitalWrite(*(segmentPinsPtr + 3), BCD[digit][3]);
    }
    if (digitValueCurrent == 0)
      digitalWrite(*digitPinsPtr, selectedState);
    else if (digitValueCurrent == 1)
    {
      if (!((*(digitsPtr + 3) == 0) && (*(digitsPtr + 2) == 0) && (*(digitsPtr + 1) == 0)))
      digitalWrite(*(digitPinsPtr + 1), selectedState);
    }
    else if (digitValueCurrent == 2)
    {
      if (!((*(digitsPtr + 3) == 0) && (*(digitsPtr + 2) == 0)))
        digitalWrite(*(digitPinsPtr + 2), selectedState);
    }
    else if (digitValueCurrent == 3)
    {
      if (*(digitsPtr + 3) != 0)
        digitalWrite(*(digitPinsPtr + 3), selectedState);
    }
    digitValuePrevious = digitValueCurrent;
    digitValueCurrent++;
    if (digitValueCurrent == digitsNo)
      digitValueCurrent = 0;
  }
}

void Multiplex7SegBCD::setValue(int value)
{
  *(digitsPtr + 3) = value / 1000;
  value %= 1000;
  *(digitsPtr + 2) = value / 100;
  value %= 100;
  *(digitsPtr + 1) = value / 10;
  *digitsPtr = value % 10;
}


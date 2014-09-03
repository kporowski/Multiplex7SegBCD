#ifndef MULTIPLEX7SEGBCD_H
#define MULTIPLEX7SEGBCD_H

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

#include "Arduino.h"

class Multiplex7SegBCD {
  public:
    Multiplex7SegBCD(byte noOfDigits, byte * digitPins, byte *bcd4Pins, byte segmentSelectedState);
    Multiplex7SegBCD(byte noOfDigits, byte * digitPins, byte *segmentPins, byte *bcd4Pins, byte segmentSelectedState);
    void tick();
    void setValue(int value);
  private:
    unsigned long msecs;
    byte digits[4];
    byte digitValueCurrent;
    byte digitValuePrevious;
    byte digitsNo;
    byte *digitsPtr; 
    byte *digitPinsPtr;
    byte *segmentPinsPtr; 
    byte bcdPinsPresent;
    byte selectedState;
    byte unselectedState;
    unsigned long lastUpdate;
};
#endif

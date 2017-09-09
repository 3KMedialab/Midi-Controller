/* 
    Component.h - Class for define a component which is connected to the Arduino board.
    Created by: Daniel Segura, August 16, 2017
*/

#ifndef AnalogComponent_h
#define AnalogComponent_h

#include "Arduino.h"

class AnalogComponent
{
  public:
    AnalogComponent(uint8_t pin);  
    uint8_t getPin();                               
      
  protected:
    uint8_t _pin;           
};
#endif
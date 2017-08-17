/* 
    Component.h - Class for define a component which is connected to the Arduino board.
    Created by: Daniel Segura, August 16, 2017
*/

#ifndef InputComponent_h
#define InputComponent_h

#include "Arduino.h"

class InputComponent
{
  public:
    InputComponent(uint8_t pin);                    // constructor for analog input components
    InputComponent(uint8_t pin, uint8_t puEnable);  // constructor for digital components
    uint8_t getPin();                               // return the pin assigned to the input component
      
  protected:
    uint8_t _pin;                                   //arduino pin number
    uint8_t _puEnable;                              //internal pullup resistor enabled
};
#endif
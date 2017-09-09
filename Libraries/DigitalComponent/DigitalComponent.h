/* 
    DigitalComponent.h - Class for define a component which is connected to the Arduino board.
    Created by: Daniel Segura, August 16, 2017
*/

#ifndef DigitalComponent_h
#define DigitalComponent_h

#include "Arduino.h"
#include "ComponentType.h"

class DigitalComponent
{
  public:
    DigitalComponent(uint8_t pin, uint8_t type, uint8_t puEnable);  
    uint8_t getPin();                               
      
  protected:
    uint8_t _pin;                                  
    uint8_t _puEnable;                              
};
#endif
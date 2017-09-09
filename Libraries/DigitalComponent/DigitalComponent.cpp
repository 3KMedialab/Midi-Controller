/*
  DigitalComponent.cpp - Class for define a component which is connected to the Arduino board.
  Created by: Daniel Segura, August 16, 2017
*/

#include "DigitalComponent.h"


DigitalComponent::DigitalComponent(uint8_t pin, uint8_t type, uint8_t puEnable)
{
  _pin = pin;
  _puEnable = puEnable;

  switch (type)
  {    
    case ComponentType::INPUT_DIGITAL :
      if (puEnable == 0)
        pinMode(_pin, INPUT);
      else 
        pinMode(_pin, INPUT_PULLUP);  
    break;

    case ComponentType::OUTPUT_DIGITAL :
      pinMode(_pin, OUTPUT);
    break;
  }  
}

/**
  Returns the value of the assigned pin. 
*/
uint8_t DigitalComponent::getPin()
{
  return _pin;
}
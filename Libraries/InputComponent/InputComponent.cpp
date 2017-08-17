/*
  Component.cpp - Class for define a component which is connected to the Arduino board.
  Created by: Daniel Segura, August 16, 2017
*/

#include "InputComponent.h"

/**
  Constructor for analog input components.

  @pin number of the pin assigned to the input analog component.
*/

InputComponent::InputComponent(uint8_t pin)
{  
  _pin = pin;
}

/**
  Constructor for analog input components.

  @pin number of the pin assigned to the input analog component.
  @puEnable 1 = INPUT_PULLUP (for use of the internal board pullup resistor, 0 = INPUT)
*/
InputComponent::InputComponent(uint8_t pin, uint8_t puEnable)
{  
  _pin = pin;
  _puEnable = puEnable;

  if (puEnable == 0)
    pinMode(_pin, INPUT);
  else 
    pinMode(_pin, INPUT_PULLUP);  
}

/**
  Returns the value of the assigned pin. 
*/
uint8_t InputComponent::getPin()
{
  return _pin;
}
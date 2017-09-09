/*
  Component.cpp - Class for define a component which is connected to the Arduino board.
  Created by: Daniel Segura, August 16, 2017
*/

#include "AnalogComponent.h"

AnalogComponent::AnalogComponent(uint8_t pin)
{
  _pin = pin; 
}

/**
  Returns the value of the assigned pin. 
*/
uint8_t AnalogComponent::getPin()
{
  return _pin;
}
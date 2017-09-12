/*
 * Component.h
 *
 * Class that represens an Input/Output component connected to Arduino
 *
 * Copyright 2017 3K MEDIALAB
 *   
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef Component_h
#define Component_h

#include "Arduino.h"
#include "ComponentType.h"

class Component
{
  public:
    Component(uint8_t pin, uint8_t type);  
    Component(uint8_t pin, uint8_t type, uint8_t puEnable);  
    uint8_t getPin();                               
      
  protected:
    uint8_t _pin;           
};
#endif
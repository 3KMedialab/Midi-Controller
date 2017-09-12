/*
 * Led.cpp
 *
 * Class that represents a Led output component.
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

#include <Led.h>

/*
* Constructor
* pin: the pin on which the Led is connected to Arduino.
*/
Led::Led(uint8_t pin) : Component (pin, ComponentType::OUTPUT_DIGITAL)
{
    _state = LOW;
    digitalWrite(pin, _state);
}

/*
* Constructor
* pin: the pin on which the Led is connected to Arduino.
* state: indicates if the Led is HIGH or LOW
*/
Led::Led(uint8_t pin, uint8_t state) : Component (pin, ComponentType::OUTPUT_DIGITAL)
{
    _state = state;
    digitalWrite(pin, _state);
}

/*
* Assign a state to the Led
* state: indicates if the Led is HIGH or LOW
*/
void Led::setState(uint8_t state)
{
    _state = state;
    digitalWrite(_pin, _state);
}

/*
* Returns the state of the Led
*/
uint8_t Led::getState()
{
    return _state;    
}
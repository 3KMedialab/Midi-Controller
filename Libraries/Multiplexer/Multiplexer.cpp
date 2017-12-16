/*
 * Multiplexer.cpp
 *
 * Class that represents a Multiplexer.
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
#include "Multiplexer.h"

/*
* Constructor
* pin: Is the output Arduino pin of the Multiplexer.
* numControlPins: number of control pins for select an input.
* controlPins: list of the control pins
* type: is the type of the components connected to the Multiplexer.
* puEnable: True for use internal PULLUP resistor in the mux output pin of Arduino. False otherwise.
*/ 
Multiplexer::Multiplexer(uint8_t pin, uint8_t numControlPins, const uint8_t * controlPins, uint8_t type, uint8_t puEnable) : Component(pin, type, puEnable)
{
    _numControlPins = numControlPins;
    _controlPins = controlPins;	
    _type = type;

    // set the control pins mode
    for (int i=0; i<numControlPins; i++)
    {
        pinMode(controlPins[i], OUTPUT);
    }  
}

/*
* Constructor
* pin: Is the output Arduino pin of the Multiplexer.
* numControlPins: number of control pins for select an input.
* controlPins: list of the control pins
* type: is the type of the components connected to the Multiplexer.
*/ 
Multiplexer::Multiplexer(uint8_t pin, uint8_t numControlPins, const uint8_t * controlPins, uint8_t type) : Component(pin, type)
{
    _numControlPins = numControlPins;
    _controlPins = controlPins;	
    _type = type;

    // set the control pins mode
    for (int i=0; i<numControlPins; i++)
    {
        pinMode(controlPins[i], OUTPUT);
    }    
}

uint8_t Multiplexer::getNumControlPins()
{
    return _numControlPins;
}

const uint8_t * Multiplexer::getControlPins()
{
    return _controlPins;
}

uint8_t Multiplexer::getType()
{
    return _type;
}

void Multiplexer::setChannel(uint8_t channel)
{
    for (int i=0; i<_numControlPins; i++)
    {
        digitalWrite(_controlPins[i], bitRead(channel, i));
    }
    
}


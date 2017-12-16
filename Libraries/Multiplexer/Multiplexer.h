/*
 * Multiplexer.h
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
#ifndef Multiplexer_h
#define Multiplexer_h

#include "Component.h"

class Multiplexer : public Component {
    
    public:
        Multiplexer(uint8_t pin, uint8_t numControlPins, const uint8_t * controlPins, uint8_t type, uint8_t puEnable);
        Multiplexer(uint8_t pin, uint8_t numControlPins, const uint8_t * controlPins, uint8_t type);
        uint8_t getNumControlPins();
        const uint8_t * getControlPins();	
        uint8_t getType();
        void setChannel(uint8_t channel);
		
    private:
        uint8_t _numControlPins;
        const uint8_t * _controlPins;	 
        uint8_t _type;
};
#endif
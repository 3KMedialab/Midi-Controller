/*
 * Led.h
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

#ifndef Led_h
#define Led_h

#include "Component.h"

class Led : public Component
{
    public:
        Led(uint8_t pin, uint8_t state);
        Led(uint8_t pin);
        void setState (uint8_t state);
        uint8_t getState();
    
    private:             
        uint8_t _state;       
};
#endif
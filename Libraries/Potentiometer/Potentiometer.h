/*
 * Potentiometer.h
 *
 * Class that represents an analog Potentiometer.
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
#ifndef Potentiometer_h
#define Potentiometer_h

#include "IPotentiometer.h"
#include "Component.h"

const uint8_t MAX_WINDOW_SIZE = 5;

class Potentiometer : public IPotentiometer, public Component {
	public:
		Potentiometer(uint8_t pin, uint8_t windowSize);
		uint16_t getValue();
		uint16_t getSmoothValue();	
		virtual uint8_t wasChanged();
		
	private:
	    uint16_t _analog[MAX_WINDOW_SIZE];
		uint8_t _analogPointer;
		uint8_t _maxPointer;
		uint8_t _windowSize;			
		uint16_t _lastValue;	
		uint16_t _value;			
};
#endif
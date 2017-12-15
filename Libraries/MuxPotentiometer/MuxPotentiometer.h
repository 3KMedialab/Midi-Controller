/*
 * Potentiometer.h
 *
 * Class that represents an analog Potentiometer connected to Arduino through a multiplexer.
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
#ifndef MuxPotentiometer_h
#define MuxPotentiometer_h

#include "IPotentiometer.h"
#include "MuxComponent.h"

const uint8_t MAX_MUX_WINDOW_SIZE = 20;

class MuxPotentiometer : public IPotentiometer, public MuxComponent {
	public:
		MuxPotentiometer(Multiplexer * mux, uint8_t channel, uint8_t windowSize);
		uint16_t getValue();
		uint16_t getSmoothValue();	
		virtual uint8_t wasChanged();
		uint8_t isNewSector();
		uint16_t getSector();
		void setSectors(uint16_t sectors);
		
	private:
	    uint16_t _analog[MAX_MUX_WINDOW_SIZE];
		uint8_t _analogPointer;
		uint8_t _maxPointer;
		uint8_t _windowSize;		
		uint16_t _sectors;		
		uint16_t _lastValue;
		uint16_t _lastSector;
		uint16_t _value;	
		uint16_t _sector;			
};
#endif
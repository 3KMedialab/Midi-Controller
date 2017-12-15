/*
 * MuxPotentiometer.cpp
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
#include "MuxPotentiometer.h"

MuxPotentiometer::MuxPotentiometer(Multiplexer * mux, uint8_t channel, uint8_t windowSize) : IPotentiometer() , MuxComponent (mux, channel)
{
	_lastValue = 0;
	_lastSector = 0;
	_value = 0;
	_sector = 0;
	_analogPointer = 0;
	_maxPointer = 0;
	_windowSize = (windowSize > MAX_MUX_WINDOW_SIZE) ? MAX_MUX_WINDOW_SIZE : windowSize;
	
	if (windowSize > _windowSize)
	for (int i = 0; i < windowSize; i++) {
		_analog[i] = 0;
	}
}

/*
* Returns the current value of the Potentiometer via analogRead() function.
*/
uint16_t MuxPotentiometer::getValue(){

	_mux->setChannel(_channel);    
	_value = analogRead(_mux->getPin()); 
	_sector = _value/(1024/_sectors);
	return _value;
}

/*
* Returns the smoothed current value of the Potentiometer via analogRead() function.
*/
uint16_t MuxPotentiometer::getSmoothValue()
{
	// Read the pin
	_mux->setChannel(_channel);  
	uint16_t value = analogRead(_mux->getPin());
	
	// Return if we only keep track of 1 value
	if (_windowSize == 1) {
		return value;
	}

	// Save the value to the history array	
	_analog[_analogPointer] = value;
	
	// Calculate the moving average
	uint16_t total = 0;
	for (int i = 0; i <= _maxPointer; i++) {
		total = total + _analog[i];
	}
	uint16_t avg = total / (_maxPointer + 1);
	  
	// Keep track of how many items we have in the array
	if (_maxPointer < _windowSize - 1) {
		_maxPointer++;
	}
	 
	// Update the array pointer 
	_analogPointer++;
	if (_analogPointer == _windowSize) {
		_analogPointer = 0;
	}	
	  
	// Return the average
	_value = avg;

	// calculate the current sector
	_sector = _value/(1024/_sectors);

	return _value;	
}

/*
* Indicates if the potentiometer was changed by the user.
* It uses the getSmoothValue() method in order to smooth the 
* analog reads. It also only consider that the component has changed
* if there is a difference greater than 1.
*/
uint8_t MuxPotentiometer::wasChanged ()
{
	getSmoothValue();
	uint16_t diff = _lastValue - _value;
	if (abs(diff) > 2) {
		_lastValue = _value;
		return 1;
	}
	return 0;
}

/*
* Return true if the potentiometer has moved to a different sector
*/
uint8_t MuxPotentiometer::isNewSector()
{
	if (_lastSector != _sector){
		_lastSector = _sector;
		return 1;
	}

	return 0;
}

/*
* Returns the sector corresponding to the current value of the potentiometer
*/
uint16_t MuxPotentiometer::getSector(){
	return _sector;
}

/*
* Set the desired number of sectors to divide the potentiometer value
*/
void MuxPotentiometer::setSectors(uint16_t newSectors){
	if (newSectors<1024 && newSectors>0){
		_sectors=newSectors;
	}else if (newSectors<0){
		_sectors=0;
	}else{
		_sectors=1023;
	}
}
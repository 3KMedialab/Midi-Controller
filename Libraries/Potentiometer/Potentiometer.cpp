/*
||
|| @file Potentiometer.cpp
|| @version 1.2
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making/using potentiometers
*/

#include "Potentiometer.h"

Potentiometer::Potentiometer(uint8_t pin) : IPotentiometer() , InputComponent(pin)
{
	_lastValue = 0;
	_value = 0;
	_analogPointer = 0;
	_maxPointer = 0;
	
	for (int i = 0; i < WINDOW_SIZE; i++) {
		_analog[i] = 0;
	}
}

uint16_t Potentiometer::getValue(){
	_value = analogRead(_pin); 
	return _value;
}

/*
  Perform smooting of analog input
*/
uint16_t Potentiometer::getSmoothValue()
{
	// Read the pin
	uint16_t value = analogRead(_pin);
	
	// Return if we only keep track of 1 value
	if (WINDOW_SIZE == 1) {
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
	if (_maxPointer < WINDOW_SIZE - 1) {
		_maxPointer++;
	}
	 
	// Update the array pointer 
	_analogPointer++;
	if (_analogPointer == WINDOW_SIZE) {
		_analogPointer = 0;
	}
	  
	// Retrun the average
	_value = avg;
	return _value;	
}

uint8_t Potentiometer::wasChanged ()
{
	uint16_t diff = _lastValue - _value;
	if (abs(diff) > 1) {
		_lastValue = _value;
		return 1;
	}
	return 0;
} 
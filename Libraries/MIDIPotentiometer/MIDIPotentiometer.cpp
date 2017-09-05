/*
  MIDIPotentiometer.cpp - Class that defines a MIDI Potentiometer.
  Created by: Daniel Segura, August 24, 2017
*/

#include <MIDIPotentiometer.h>

MIDIPotentiometer::MIDIPotentiometer(uint8_t pin, uint8_t windowSize, MIDIMessage * message) : Potentiometer (pin, windowSize)
{
    _message = message;
}

MIDIMessage MIDIPotentiometer::getMessage()
{
    return *_message;
}

uint8_t MIDIPotentiometer::wasChanged ()
{
	Potentiometer::getSmoothValue();
	uint16_t diff = _lastValue - _value;
	if (abs(diff) > 1) {
        _lastValue = _value;
        (*_message).setDataByte2(map(_value, 0, 1023, 0, 127));
		return 1;
	}
	return 0;
} 
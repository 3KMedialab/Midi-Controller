/*
 * MIDIPotentiometer.cpp
 *
 * Class that represents a MIDI Potentiometer
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
#include <MIDIPotentiometer.h>

/*
* Constructor
* pin: Is the Arduino pin the potentiometer is connected to. 
* windowSize: number of measures of the potentiometer to smooth the reads.
* message: the MIDI message assigned to the component.
*/
MIDIPotentiometer::MIDIPotentiometer(uint8_t pin, uint8_t windowSize, MIDIMessage * message) : Potentiometer (pin, windowSize)
{
    _message = message;
}

/*
* Returns the MIDI message assigned to the component
*/
MIDIMessage MIDIPotentiometer::getMessage()
{
    return *_message;
}

/*
* Indicates if the potentiometer was changed by the user.
* It uses the getSmoothValue() method in order to smooth the 
* analog reads. It also only consider that the component has changed
* if there is a difference greater than 1. If a change was detected, the
* new value read is mapped in the range of the midi messages data bytes, and
* is assigned to the second data byte of the message.
* Returns true if the MIDI potentiometer value has changed.
*/
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
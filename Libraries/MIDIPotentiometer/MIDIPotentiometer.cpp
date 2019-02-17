/*
 * MIDIPotentiometer.cpp
 *
 * Template class that represents a MIDI Potentiometer. It can be derived from different Potentiometer implementations.
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
* Constructor for MIDI Potentiometers connected directly to Arduino board
* pin: Is the Arduino pin the potentiometer is connected to. 
* windowSize: number of measures of the potentiometer to smooth the reads.
* message: the MIDI message assigned to the component.
*/
template<class T>
MIDIPotentiometer<T>::MIDIPotentiometer(uint8_t pin, uint8_t windowSize, MIDIMessage * message) : T (pin, windowSize)
{
	_midiMessages[ACTION_MESSAGE] = *message;

	_availableMessageTypes[0] = midi::ControlChange;
	_availableMessageTypes[1] = midi::ProgramChange;
	_availableMessageTypes[2] = midi::InvalidType; 
}

/*
* Constructor for MIDI Potentiometers connected directly to Arduino board
* pin: Is the Arduino pin the potentiometer is connected to. 
* windowSize: number of measures of the potentiometer to smooth the reads.
*/
template<class T>
MIDIPotentiometer<T>::MIDIPotentiometer(uint8_t pin, uint8_t windowSize) : T (pin, windowSize)
{
	_availableMessageTypes[0] = midi::ControlChange;
	_availableMessageTypes[1] = midi::ProgramChange; 
	_availableMessageTypes[2] = midi::InvalidType;
}

/*
* Constructor for MIDI Potentiometers connected to Arduino boards through a Multiplexer
* mux: multiplexer which the MIDI Potentiometer is connected to.              
* channel: channel of the mux where the MIDI Potentiometer is connected.
* windowSize: number of measures of the potentiometer to smooth the reads.
* message: the MIDI message assigned to the component.
*/
template<class T>
MIDIPotentiometer<T>::MIDIPotentiometer(Multiplexer * mux, uint8_t channel, uint8_t windowSize, MIDIMessage * message) : T (mux, channel, windowSize)
{
	_midiMessages[ACTION_MESSAGE] = *message;

	_availableMessageTypes[0] = midi::ControlChange;
	_availableMessageTypes[1] = midi::ProgramChange;
	_availableMessageTypes[2] = midi::InvalidType; 
}

/*
* Constructor for MIDI Potentiometers connected to Arduino boards through a Multiplexer
* mux: multiplexer which the MIDI Potentiometer is connected to.              
* channel: channel of the mux where the MIDI Potentiometer is connected. 
* windowSize: number of measures of the potentiometer to smooth the reads.
*/
template<class T>
MIDIPotentiometer<T>::MIDIPotentiometer(Multiplexer * mux, uint8_t channel, uint8_t windowSize) : T (mux, channel, windowSize)
{
	_availableMessageTypes[0] = midi::ControlChange;
	_availableMessageTypes[1] = midi::ProgramChange; 
	_availableMessageTypes[2] = midi::InvalidType;
}

/*
* Returns the MIDI message that has to be sent regarding the component state
*/
template<class T>
MIDIMessage * MIDIPotentiometer<T>::getMessageToSend()
{
	if (this->wasChanged())
    {         
		switch (_midiMessages[ACTION_MESSAGE].getType())
		{
			case midi::ProgramChange:
				_midiMessages[ACTION_MESSAGE].setDataByte1(map(this->getSmoothValue(), 0, 1022, 0, 127));
			break;

			case midi::ControlChange:
				_midiMessages[ACTION_MESSAGE].setDataByte2(map(this->getSmoothValue(), 0, 1022, 0, 127));
			break;			
		}      
		
		return &(_midiMessages[ACTION_MESSAGE]);                
	}

	return NULL;
}

/*
* Returns the number of MIDI messages that the component can send
*/
template<class T>
uint8_t MIDIPotentiometer<T>::getNumMessages()
{
    return MIDI_POTENTIOMETER_NUM_MESSAGES;
}

/*
* Returns the MIDI messages that the component can send
*/
template<class T>
MIDIMessage * MIDIPotentiometer<T>::getMessages()
{
    return _midiMessages;
}

/*
* Returns the size of the MIDI data
*/
template<class T>
uint8_t MIDIPotentiometer<T>::getDataSize()
{
	return sizeof(uint8_t) * 3;
}

/*
* Returns true if the potentiometer was moved
*/
template<class T>
uint8_t MIDIPotentiometer<T>::wasActivated()
{
	return this->wasChanged();
}

/*
* Returns the list with the MIDI messages the component can handle
*/
template<class T>
uint8_t * MIDIPotentiometer<T>::getAvailableMessageTypes()
{
    return _availableMessageTypes;
}

/*
* Returns the numbers of available MIDI messages the component can handle
*/
template<class T>
uint8_t MIDIPotentiometer<T>::getNumAvailableMessageTypes()
{
    return MIDI_POTENTIOMETER_AVAILABLE_MESSAGES;
}
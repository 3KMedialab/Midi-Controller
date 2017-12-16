/*
 * MIDIPotentiometer.h
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
#ifndef MIDIPotentiometer_h
#define MIDIPotentiometer_h

#include "IMIDIComponent.h"
#include "MIDIMessage.h"
#include "MIDI.h"
#include "Multiplexer.h"

#define MIDI_POTENTIOMETER_NUM_MESSAGES 1       // number of MIDI messages the component can send
#define MIDI_POTENTIOMETER_AVAILABLE_MESSAGES 3 // number of MIDI messages the component can handle
#define ACTION_MESSAGE 0

template<class T>
class MIDIPotentiometer : public T, public IMIDIComponent
{
    public:
        MIDIPotentiometer(uint8_t pin, uint8_t windowSize);
        MIDIPotentiometer(uint8_t pin, uint8_t windowSize, MIDIMessage * message);
        MIDIPotentiometer(Multiplexer * mux, uint8_t channel, uint8_t windowSize);
        MIDIPotentiometer(Multiplexer * mux, uint8_t channel, uint8_t windowSize, MIDIMessage * message);

        MIDIMessage * getMessageToSend();
        uint8_t getNumMessages();
        MIDIMessage * getMessages();
        uint8_t getDataSize();
        uint8_t wasActivated();
        uint8_t * getAvailableMessageTypes();
        uint8_t getNumAvailableMessageTypes();
     
    private:    
        MIDIMessage _midiMessages [MIDI_POTENTIOMETER_NUM_MESSAGES];                 // array with the MIDI messages the component can send     
        uint8_t _availableMessageTypes [MIDI_POTENTIOMETER_AVAILABLE_MESSAGES];      // list with the MIDI messages the component can handle
};
#endif
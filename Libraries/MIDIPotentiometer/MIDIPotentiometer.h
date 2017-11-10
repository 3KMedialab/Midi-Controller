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

#include "Potentiometer.h"
#include "IMIDIComponent.h"
#include "MIDIMessage.h"

#define MIDI_POTENTIOMETER_NUM_MESSAGES 1 // number of MIDI messages the component can send
#define ACTION_MESSAGE 0

class MIDIPotentiometer : public Potentiometer, public IMIDIComponent
{
    public:
        MIDIPotentiometer(uint8_t pin, uint8_t windowSize);
        MIDIPotentiometer(uint8_t pin, uint8_t windowSize, MIDIMessage * message);
        uint8_t wasChanged ();

        MIDIMessage * getMessageToSend();
        uint8_t getNumMessages();
        MIDIMessage * getMessages();
        uint8_t getDataSize();
        uint8_t wasActivated();
     
    private:    
        MIDIMessage _midiMessages [MIDI_POTENTIOMETER_NUM_MESSAGES];    // array with the MIDI messages the component can send     
};
#endif
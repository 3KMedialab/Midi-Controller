/*
 * MIDIButton.h
 *
 * Class that represents a MIDI Button
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
#ifndef MIDIButton_h
#define MIDIButton_h

#include "Button.h"
#include "IMIDIComponent.h"
#include "MIDIMessage.h"
#include "MIDI.h"

#define MIDI_BUTTON_NUM_MESSAGES 2  // number of MIDI messages the component can send
#define ON_PRESSED_MESSAGE 0        
#define ON_RELEASED_MESSAGE 1
#define MIDI_BUTTON_AVAILABLE_MESSAGES 2  // number of available MIDI messages the component can handle

class MIDIButton : public Button, public IMIDIComponent
{
    public:
        MIDIButton(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime, MIDIMessage * onPressedMessage, MIDIMessage * onReleasedMessage);
        MIDIButton(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime);
        MIDIMessage * getMessageToSend();
        uint8_t getNumMessages();
        MIDIMessage * getMessages();
        uint8_t getDataSize();
        uint8_t wasActivated();
        uint8_t * getAvailableMessageTypes();
        uint8_t getNumAvailableMessageTypes();
             
    private:       
        MIDIMessage _midiMessages [MIDI_BUTTON_NUM_MESSAGES];               // array with the MIDI messages the component can send
        uint8_t _availableMessageTypes [MIDI_BUTTON_AVAILABLE_MESSAGES];    // list with the MIDI messages the component can handle
};
#endif
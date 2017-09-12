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
#include "MIDIMessage.h"

class MIDIButton : public Button
{
    public:
        MIDIButton(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime, MIDIMessage * onPressedMessage, MIDIMessage * onReleasedMessage);
        MIDIMessage getOnPressedMessage();
        MIDIMessage getOnReleasedMessage();
     
    private:
        MIDIMessage *_onPressedMessage;
        MIDIMessage *_onReleasedMessage;       
};
#endif
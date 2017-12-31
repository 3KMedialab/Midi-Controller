/*
 * MIDIButton.cpp
 *
 * Template class that represents a MIDI Button. It can be derived from different Button implementations.
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

#include <MIDIButton.h>

/*
* Constructor for MIDI buttons connected directly to Arduino board
* pin: Is the Arduino pin the button is connected to.              
* puEnable: Enables the AVR internal pullup resistor if != 0 (can also use true or false).                                         
* invert: If invert == 0, interprets a high state as pressed, low as released. If invert != 0, interprets a high state as        
*         released, low as pressed  (can also use true or false).     
* dbTime: Is the debounce time in milliseconds.
* onPressedMessage: MIDI message that will be send when the button is pressed.
* onReleasedMessage: MIDI message that will be send when the button is released.                                                                    
* (Note that invert cannot be implied from puEnable since an external  
*  pullup could be used.)                                              
*/
template<class C>
MIDIButton<C>::MIDIButton(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime, MIDIMessage * onPressedMessage, MIDIMessage * onReleasedMessage) : C (pin, puEnable, invert, dbTime)
{   
    _midiMessages[ON_PRESSED_MESSAGE] = *onPressedMessage;
    _midiMessages[ON_RELEASED_MESSAGE] = *onReleasedMessage;

    _availableMessageTypes[0] = midi::NoteOn;
    _availableMessageTypes[1] = midi::NoteOff;    
    _availableMessageTypes[2] = midi::InvalidType;
}

/*
* Constructor for MIDI buttons connected directly to Arduino board
* pin: Is the Arduino pin the button is connected to.              
* puEnable: Enables the AVR internal pullup resistor if != 0 (can also use true or false).                                         
* invert: If invert == 0, interprets a high state as pressed, low as released. If invert != 0, interprets a high state as        
*         released, low as pressed  (can also use true or false).     
* dbTime: Is the debounce time in milliseconds.
* (Note that invert cannot be implied from puEnable since an external  
*  pullup could be used.)                                              
*/
template<class C>
MIDIButton<C>::MIDIButton(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime) : C (pin, puEnable, invert, dbTime)
{
    _availableMessageTypes[0] = midi::NoteOn;
    _availableMessageTypes[1] = midi::NoteOff;
    _availableMessageTypes[2] = midi::InvalidType;
}

/*
* Constructor for MIDI buttons connected to Arduino board through a multiplexer
* mux: multiplexer which the MIDI Button is connected to.              
* channel: channel of the mux where the MIDI Button is connected                                         
* invert: If invert == 0, interprets a high state as pressed, low as released. If invert != 0, interprets a high state as        
*         released, low as pressed  (can also use true or false).     
* dbTime: Is the debounce time in milliseconds.
* onPressedMessage: MIDI message that will be send when the button is pressed.
* onReleasedMessage: MIDI message that will be send when the button is released. 
* (Note that invert cannot be implied from puEnable since an external  
*  pullup could be used.)                                              
*/
template<class C>
MIDIButton<C>::MIDIButton(Multiplexer * mux, uint8_t channel, uint8_t invert, uint32_t dbTime, MIDIMessage * onPressedMessage, MIDIMessage * onReleasedMessage) : C (mux, channel, invert, dbTime)
{
    _midiMessages[ON_PRESSED_MESSAGE] = *onPressedMessage;
    _midiMessages[ON_RELEASED_MESSAGE] = *onReleasedMessage;

    _availableMessageTypes[0] = midi::NoteOn;
    _availableMessageTypes[1] = midi::NoteOff;
    _availableMessageTypes[2] = midi::InvalidType;
}

/*
* Constructor for MIDI buttons connected to Arduino board through a multiplexer
* mux: multiplexer which the MIDI Button is connected to.              
* channel: channel of the mux where the MIDI Button is connected                                         
* invert: If invert == 0, interprets a high state as pressed, low as released. If invert != 0, interprets a high state as        
*         released, low as pressed  (can also use true or false).     
* dbTime: Is the debounce time in milliseconds.
* (Note that invert cannot be implied from puEnable since an external  
*  pullup could be used.)                                              
*/
template<class C>
MIDIButton<C>::MIDIButton(Multiplexer * mux, uint8_t channel, uint8_t invert, uint32_t dbTime) : C (mux, channel, invert, dbTime)
{
    _availableMessageTypes[0] = midi::NoteOn;
    _availableMessageTypes[1] = midi::NoteOff;
    _availableMessageTypes[2] = midi::InvalidType;
}

/*
* Returns the MIDI message that has to be sent regarding the component state
*/
template<class C>
MIDIMessage * MIDIButton<C>::getMessageToSend()
{
    this->read();
    
    if (this->wasPressed())
    {
        return &(_midiMessages[ON_PRESSED_MESSAGE]);              
    }
    
    if (this->wasReleased())
    {
        return &(_midiMessages[ON_RELEASED_MESSAGE]);              
    }    
    
    return NULL;
}

/*
* Returns the number of MIDI messages that the component can send
*/
template<class C>
uint8_t MIDIButton<C>::getNumMessages()
{
    return MIDI_BUTTON_NUM_MESSAGES;
}

/*
* Returns the MIDI messages that the component can send
*/
template<class C>
MIDIMessage * MIDIButton<C>::getMessages()
{
    return _midiMessages;
}

/*
* Returns the size of the MIDI data
*/
template<class C>
uint8_t MIDIButton<C>::getDataSize()
{
    return (sizeof(uint8_t) * 4) * MIDI_BUTTON_NUM_MESSAGES;
}

/*
* Returns true if the button was pressed
*/
template<class C>
uint8_t MIDIButton<C>::wasActivated()
{
    this->read();
    return this->wasPressed();     
}

/*
* Returns the list with the MIDI messages the component can handle
*/
template<class C>
uint8_t * MIDIButton<C>::getAvailableMessageTypes()
{
    return _availableMessageTypes;
}

/*
* Returns the numbers of available MIDI messages the component can handle
*/
template<class C>
uint8_t MIDIButton<C>::getNumAvailableMessageTypes()
{
    return MIDI_BUTTON_AVAILABLE_MESSAGES;
}
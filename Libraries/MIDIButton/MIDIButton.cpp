/*
 * MIDIButton.cpp
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

#include <MIDIButton.h>

/*
* Constructor
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
MIDIButton::MIDIButton(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime, MIDIMessage * onPressedMessage, MIDIMessage * onReleasedMessage) : Button (pin, puEnable, invert, dbTime)
{
    _onPressedMessage = onPressedMessage;
    _onReleasedMessage = onReleasedMessage;
}

/*
* Returns the MIDI message assigned to the onPressed event.
*/
MIDIMessage MIDIButton::getOnPressedMessage()
{
    return *_onPressedMessage;
}

/*
* Returns the MIDI message assigned to the onReleased event.
*/
MIDIMessage MIDIButton::getOnReleasedMessage()
{
    return *_onReleasedMessage;
}
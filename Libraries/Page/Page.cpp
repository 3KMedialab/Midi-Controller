/*
 * Page.cpp
 *
 * Class that represents a layout of MIDIMessages assigned to components. 
 * Pages are stored/loaded to/from the EEPROM
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
#include "Page.h"

void Page::save(uint8_t numPage, MIDIButton * midiButtons, MIDIPotentiometer * midiPots)
{
    //get the begin address of the page
    uint16_t address = 0;
    
    if (numPage != 0)
    {
        address = _size * numPage; 
    }
    
    // save the MIDI messages assigned to the MIDI Buttons to the EEPROM
    for (uint8_t i = 0; i < NUM_MIDI_BUTTONS-1; i++)
    {
        saveMIDIButton(&address, midiButtons[i]);        
    }

    // save the MIDI messages assigned to the MIDI Potentiomenters to the EEPROM
    for (uint8_t i = 0; i < NUM_MIDI_POTS-1; i++)
    {
        saveMIDIPotentiometer(&address, midiPots[i]);         
    }
}

void Page::load(uint8_t numPage, MIDIButton * midiButtons, MIDIPotentiometer * midiPots)
{
    //get the begin address of the page
    uint16_t address = 0;
    
    if (numPage != 0)
    {
        address = _size * numPage; 
    }

}

void Page::saveMIDIButton(uint16_t * address, MIDIButton button)
{
    // save the id of the MIDI Button
    saveId(address, button.getId());
     
    // save the onPressed MIDI message
    saveMIDIMessage(address, button.getOnPressedMessage());

    // save the id of the MIDI Button
    saveId(address, button.getId());
    
    // save the onPressed MIDI message
    saveMIDIMessage(address, button.getOnReleasedMessage());   
}

void Page::saveMIDIPotentiometer(uint16_t * address, MIDIPotentiometer pot)
{   
    // save the id of the MIDI Potentiometer
    saveId(address, pot.getId());
     
    // save the MIDI message
    saveMIDIMessage(address, pot.getMessage()); 
}

void Page::saveMIDIMessage(uint16_t * address, MIDIMessage message)
{
    EEPROM.update(*address,message.getType());
    (*address) += sizeof(uint8_t);
    EEPROM.update(*address,message.getDataByte1());
    (*address) += sizeof(uint8_t);
    EEPROM.update(*address,message.getDataByte2());
    (*address) += sizeof(uint8_t);
    EEPROM.update(*address,message.getChannel());
    (*address) += sizeof(uint8_t);
}

void Page::saveId(uint16_t * address, uint8_t id)
{
    EEPROM.update(*address,id);
    (*address) += sizeof(uint8_t);    
}




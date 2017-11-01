/*
 * MemoryManager.h
 *
 * Class that manage the interactions between the MIDI controller and the EEPROM
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

#include "MemoryManager.h"

/*
* Initializes the memory manager regarding the number of MIDI components that will be managed
* midiComponents: list of the MID Icomponents that will be managed
* numMIDIComponents: number of MIDI components
*/
void MemoryManager::initialize(IMIDIComponent ** midiComponents, uint8_t numMIDIComponents)
{
    // calculate the page size in bytes in EEPROM
    for (uint8_t i = 0; i < numMIDIComponents; i++)
    {
        _pageSize += midiComponents[i]->getDataSize();
    }

    // calculate the maximum number of pages
    _maxNumPages = EEPROM_SIZE / _pageSize;
}

/*
* Saves the MIDI messages assigned to the MIDI components in a page into the EEPROM
* page: page number where the data will be stored
* midiComponents: list of the MID Icomponents that will be managed
* numMIDIComponents: number of MIDI components
*/
void MemoryManager::saveMIDIComponents(uint8_t page, IMIDIComponent ** midiComponents, uint8_t numMIDIComponents)
{
    //get the begin address of the page
    uint16_t address = 0;
    
    if (page > 1)
    {
        address = _pageSize * (page-1); 
    }

    // save the MIDI messages assigned to each MIDI component into the EEPROM
    for (uint8_t i = 0; i < numMIDIComponents; i++)
    {
        saveMIDIComponent(&address, midiComponents[i]);        
    }
}

/*
* Save the MIDI messages assigned to a MIDI component into the EEPROM
* address: start addres of the EEPROM where the MIDI messages will be stored.
* midiComponent: MIDI component which MIDI messages will be stored.
*/
void MemoryManager::saveMIDIComponent(uint16_t * address, IMIDIComponent * midiComponent)
{    
    for (int i = 0; i < midiComponent->getNumMessages(); i++)
    {       
        saveMIDIMessage(address, midiComponent->getMessages()[i]);
    }       
}

/*
* Saves a MIDI message into the EEPROM
* address: start addres of the EEPROM where the MIDI message will be stored.
* message: the MIDI message that will be stored
*/
void MemoryManager::saveMIDIMessage(uint16_t * address, MIDIMessage message)
{
    EEPROM.update((*address),message.getType());
    (*address) += sizeof(uint8_t);
    EEPROM.update((*address),message.getDataByte1());
    (*address) += sizeof(uint8_t);
    EEPROM.update((*address),message.getDataByte2());
    (*address) += sizeof(uint8_t);
    EEPROM.update((*address),message.getChannel());
    (*address) += sizeof(uint8_t);
}

/*
* Load the MIDI messages stored in a page into the MIDI components
* page: page number where the data is stored
* midiComponents: list of the MID Icomponents that will be managed
* numMIDIComponents: number of MIDI components
*/
void MemoryManager::loadMIDIComponents(uint8_t page, IMIDIComponent ** midiComponents, uint8_t numMIDIComponents)
{
    //get the begin address of the page
    uint16_t address = 0;
    
    if (page > 1)
    {
        address = _pageSize * (page-1); 
    }

    // save the MIDI messages assigned to each MIDI component into the EEPROM
    for (uint8_t i = 0; i < numMIDIComponents; i++)
    {
        loadMIDIComponent(&address, midiComponents[i]);        
    }
}

/*
* Load the MIDI messages into a MIDI component
* address: start addres of the EEPROM where the MIDI messages are stored.
* midiComponent: MIDI component which MIDI messages will be loaded into.
*/
void MemoryManager::loadMIDIComponent(uint16_t * address, IMIDIComponent * midiComponent)
{    
    for (int i = 0; i < midiComponent->getNumMessages(); i++)
    {       
        loadMIDIMessage(address, &(midiComponent->getMessages()[i]));
    }       
}

/*
* Load a MIDI message from the EEPROM
* address: start addres of the EEPROM where the MIDI message is stored.
* message: the MIDI message that will be loaded with the data from the EEPROM
*/
void MemoryManager::loadMIDIMessage(uint16_t * address, MIDIMessage * message)
{
    message->setType(EEPROM.read((*address)));
    (*address) += sizeof(uint8_t);
    message->setDataByte1(EEPROM.read((*address)));
    (*address) += sizeof(uint8_t);
    message->setDataByte2(EEPROM.read((*address)));
    (*address) += sizeof(uint8_t);
    message->setChannel(EEPROM.read((*address)));
    (*address) += sizeof(uint8_t);
}

/*
* Returns the maximum page numbers that the MemoryManager can manage
*/
uint8_t MemoryManager::getMaxPages()
{
    return _maxNumPages;
}
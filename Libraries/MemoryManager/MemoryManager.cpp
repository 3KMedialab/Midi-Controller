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
* Return: 0 if the total components size don't fit into the EEPROM, 1 otherwise
* midiComponents: list of the MIDI components that will be managed
* numMIDIComponents: number of MIDI components
* sequenceLength: number of steps in a sequence
* stepSize: size of each step on a sequence
* globalConfigSize: size of the controller global configuration
*/
uint8_t MemoryManager::initialize(IMIDIComponent ** midiComponents, uint8_t numMIDIComponents, uint8_t sequenceLength, uint8_t stepSize, uint8_t globalConfigSize)
{
    // calculate the page size in bytes in EEPROM
    for (uint8_t i = 0; i < numMIDIComponents; i++)
    {
        _pageSize += midiComponents[i]->getDataSize();
    }
	
	// calculate the sequence size in bytes in EEPROM
	_sequenceSize = sequenceLength * stepSize;
	
	_globalConfigSize = globalConfigSize;		

    // calculate the total size of data in bytes that will be stored into EEPROM and check if it fits
	if (((_pageSize * NUM_PAGES) + (_sequenceSize * NUM_SEQUENCES) + globalConfigSize) > MEMORY_SIZE)
	{
		return 0;
	}
	
	return 1;    
}

/*
* Load the global configuration parameters from EEPROM
* globalConfig: object in which the configuration will be loaded
*/
void MemoryManager::loadGlobalConfiguration(GlobalConfig * globalConfig)
{
    globalConfig->setMIDIChannel(EEPROM.read(0));
	globalConfig->setSequencerMIDIChannel(EEPROM.read(1));
    globalConfig->setMode(EEPROM.read(2));
    globalConfig->setRootNote(EEPROM.read(3));
    globalConfig->setSendClockWhilePlayBack(EEPROM.read(4));
}

/*
* Save the global configuration parameters to EEPROM
* globalConfig: configuration to be saved
*/
void MemoryManager::saveGlobalConfiguration(GlobalConfig globalConfig)
{
    EEPROM.update(0, globalConfig.getMIDIChannel());
	EEPROM.update(1, globalConfig.getSequencerMIDIChannel());
    EEPROM.update(2, globalConfig.getMode());
    EEPROM.update(3, globalConfig.getRootNote());
    EEPROM.update(4, globalConfig.getSendClockWhilePlayBack());
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
    uint16_t address = _globalConfigSize;
    
    if (page > 1)
    {
        address += _pageSize * (page-1); 
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
}

/*
* Saves the steps within a sequence into the EEPROM
* numSequence: sequence number that will be stored
* sequence: list of the steps that will be stored
* sequenceLength: number of steps that will be stored
*/
void MemoryManager::saveSequence(uint8_t numSequence, Step * sequence, uint8_t sequenceLength)
{
    //calculate the start address of the sequence
    uint16_t address = _globalConfigSize + (_pageSize * NUM_PAGES);
    
    if (numSequence > 1)
    {
        address += _sequenceSize * (numSequence-1); 
    }

    // save the steps of the sequence into the EEPROM
    for (uint8_t i = 0; i < sequenceLength; i++)
    {
        saveStep(&address, sequence[i]);        
    }
}

/*
* Saves a step of a sequence into the EEPROM
* address: start addres of the EEPROM where the step will be stored.
* step: the step that will be stored
*/
void MemoryManager::saveStep(uint16_t * address, Step step)
{
    EEPROM.update((*address),step.getNote());
    (*address) += sizeof(uint8_t);
    EEPROM.update((*address),step.isEnabled());
    (*address) += sizeof(uint8_t);
    EEPROM.update((*address),step.isLegato());
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
    uint16_t address = _globalConfigSize;
    
    if (page > 1)
    {
        address += _pageSize * (page-1); 
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
}

/*
* Load the steps data into a sequence
* numSequence: sequence number that will be loaded
* sequence: sequence that will be loaded with the steps data
* sequenceLength: number of steps that will be loaded
*/
void MemoryManager::loadSequence(uint8_t numSequence, Step * sequence, uint8_t sequenceLength)
{
    //get the begin address of the sequence
    uint16_t address = _globalConfigSize + (_pageSize * NUM_PAGES);
    
    if (numSequence > 1)
    {
        address += _sequenceSize * (numSequence-1); 
    }
  
    // load the steps into the sequence
    for (uint8_t i = 0; i < sequenceLength; i++)
    {      
        loadStep(&address, &(sequence[i]));        
    }
}

/*
* Load a step from the EEPROM
* address: start addres of the EEPROM where the step is stored.
* step: the step that will be loaded with the data from the EEPROM
*/
void MemoryManager::loadStep(uint16_t * address, Step * step)
{
    step->setNote(EEPROM.read((*address)));
    (*address) += sizeof(uint8_t);
    step->setEnabled(EEPROM.read((*address)));
    (*address) += sizeof(uint8_t);
    step->setLegato(EEPROM.read((*address)));
    (*address) += sizeof(uint8_t);   
}
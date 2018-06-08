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
#ifndef MemoryManager_h
#define MemoryManager_h

#include <EEPROM.h>
#include <IMIDIComponent.h> 
#include <GlobalConfig.h>

#define MEMORY_SIZE 1024

class MemoryManager
{
  public:   
    void initialize(IMIDIComponent ** midiComponents, uint8_t numMIDIComponents);
    void saveMIDIComponents(uint8_t page, IMIDIComponent ** midiComponents, uint8_t numMIDIComponents);
    void loadMIDIComponents(uint8_t page, IMIDIComponent ** midiComponents, uint8_t numMIDIComponents);
    void loadGlobalConfiguration(GlobalConfig * globalConfig);
    void saveGlobalConfiguration(GlobalConfig globalConfig);
    uint8_t getMaxPages();

  private:
    uint8_t _pageSize;        // size of a MIDI messages page regarding the number of MIDI components                        
    uint8_t _maxNumPages;     // maximum number of pages of MIDI data that can be managed

    void saveMIDIComponent(uint16_t * address , IMIDIComponent * midiComponent);
    void saveMIDIMessage(uint16_t * address, MIDIMessage message); 
    void loadMIDIComponent(uint16_t * address , IMIDIComponent * midiComponent);
    void loadMIDIMessage(uint16_t * address, MIDIMessage * message); 
};
#endif
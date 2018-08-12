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
#include <ControllerConfig.h>
#include <Step.h>

#define MEMORY_SIZE 1024

class MemoryManager
{
  public:   
    uint8_t initialize(IMIDIComponent ** midiComponents, uint8_t numMIDIComponents, uint8_t sequenceLength, uint8_t stepSize, uint8_t globalConfigSize);
    void saveMIDIComponents(uint8_t page, IMIDIComponent ** midiComponents, uint8_t numMIDIComponents);
	void saveSequence(uint8_t numSequence, Step * sequence, uint8_t sequenceLength);
    void loadMIDIComponents(uint8_t page, IMIDIComponent ** midiComponents, uint8_t numMIDIComponents);
	void loadSequence(uint8_t numSequence, Step * sequence, uint8_t sequenceLength);
    void loadGlobalConfiguration(GlobalConfig * globalConfig);
    void saveGlobalConfiguration(GlobalConfig globalConfig);

  private:
    uint8_t _pageSize;        // size of a MIDI messages page regarding the number of MIDI components 
	uint8_t _sequenceSize; // size of a sequence regarding the nimber of steps
	uint8_t _globalConfigSize;	// size of the global configuration object

    void saveMIDIComponent(uint16_t * address , IMIDIComponent * midiComponent);
    void saveMIDIMessage(uint16_t * address, MIDIMessage message); 
	void saveStep(uint16_t * address, Step step);
    void loadMIDIComponent(uint16_t * address , IMIDIComponent * midiComponent);
    void loadMIDIMessage(uint16_t * address, MIDIMessage * message); 
	void loadStep(uint16_t * address, Step * step);
};
#endif
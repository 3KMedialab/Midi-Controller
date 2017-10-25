/*
 * Page.h
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
#ifndef Page_h
#define Page_h

#include "MIDIButton.h"
#include "MIDIPotentiometer.h"
#include "EEPROM.h"
#include "ControllerConfig.h"

const uint8_t _size = (MIDIButton::getDataSize() * NUM_MIDI_BUTTONS) + (MIDIPotentiometer::getDataSize() * NUM_MIDI_POTS);  

class Page
{
  public:       

    static void save(uint8_t numPage, MIDIButton * midiButtons, MIDIPotentiometer * midiPots);
    static void load(uint8_t numPage, MIDIButton * midiButtons, MIDIPotentiometer * midiPots);

  private:
    
    static void saveMIDIButton(uint16_t * address, MIDIButton button);
    static void saveMIDIPotentiometer(uint16_t * address, MIDIPotentiometer pot);
    static void saveMIDIMessage(uint16_t * address, MIDIMessage message);
    static void saveId(uint16_t * address, uint8_t id);
};
#endif
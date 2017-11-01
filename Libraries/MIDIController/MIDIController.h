/*
 * MIDIController.h
 *
 * The MIDI Controller class. It contains an array of MIDIButtons and MIDIPotentiometers. 
 * It also contains a button that implemts the shift function and two leds to indicate 
 * the current shift mode. 
 *
 * The configuration of the controller is stored in the ControllerConfig.h file
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

#ifndef MIDIController_h
#define MIDIController_h

#include <MIDI.h>
#include <IMIDIComponent.h> 
#include <ControllerConfig.h>
#include <MemoryManager.h>
#include <Button.h>

typedef midi::MidiInterface<HardwareSerial> MidiInterface;

class MIDIController
{
  public:   
    MIDIController(MidiInterface& inInterface, IMIDIComponent ** components, uint8_t numMIDIComponents);
    MIDIController(IMIDIComponent ** components, uint8_t numMIDIComponents);
      
    void begin(); 
    void processMIDIComponents(); 
    void processPageButtons();   

  private:
    uint8_t _numMIDIComponents;                                                         // number of MIDI components the controller will manage
    IMIDIComponent ** _midiComponents;                                                  // MIDI components the controller will manage

    MemoryManager _memoryManager;                                                       // object to manage interactions between the controller and the EEPROM
    uint8_t _currentPage;                                                               // current page of MIDI messages loaded into the controller

    Button _decPageButton = Button(DEC_PAGE_BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);   // button for load the previous page of MIDI messages into the MIDI components
    Button _incPageButton = Button(INC_PAGE_BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);   // button for load the next page of MIDI messages into the MIDI components
    
    MidiInterface& _mMidi;                                                              // object to manage the MIDI functionality

    void processMidiComponent(IMIDIComponent * component);
    void sendMIDIMessage(MIDIMessage * message);
    void printSerial(MIDIMessage message);
    void savePage(uint8_t page);
    void loadPage(uint8_t page);
};
#endif
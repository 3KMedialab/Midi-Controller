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

#include <MidiWorker.h>
#include <IMIDIComponent.h> 
#include <Pitches.h>
#include <ControllerConfig.h>
#include <MemoryManager.h>
#include <ScreenManager.h>
#include <Button.h>
#include <Potentiometer.h>
#include <Led.h>
#include <GlobalConfig.h>
#include <Sequencer.h>
#include <SyncManager.h>

#define MICROSECONDS_PER_MINUTE 60000000

class MIDIController
{
  public:   
    MIDIController(MidiWorker * worker, IMIDIComponent ** components, uint8_t numMIDIComponents);
    MIDIController(IMIDIComponent ** components, uint8_t numMIDIComponents);
      
    void begin(); 
    void processMIDIComponents(); 
    void processIncDecButtons();
    void processSelectValuePot();
    void processMultiplePurposeButton();
    void processEditModeButton();
    void playBackSequence();
    void processOperationModeButton();
    void sendMIDIClock();
    void updateBpmIndicatorStatus();
    void updateSyncStatus();

  private:
    uint8_t _numMIDIComponents;                                                                         // number of MIDI components the controller will manage
    IMIDIComponent ** _midiComponents;                                                                  // MIDI components the controller will manage

    MemoryManager _memoryManager;                                                                       // object to manage interactions between the controller and the EEPROM
    uint8_t _currentPage;                                                                               // current page of MIDI messages loaded into the controller
    uint8_t _wasPageSaved;                                                                              // flag that indicates wether a page was saved or not.	  
    uint8_t _wasSequenceSaved;                                                                          // flag that indicates wether a sequence was saved or not.
    uint8_t _wasGlobalConfigSaved;                                                                      // flag that indicates wether global configuration was saved or not.
    uint8_t _accesToGloabalEdit;                                                                        // flag that indicates wether we have just accesed to edit global config or not.

    ScreenManager _screenManager;                                                                       // object to manage interactions between the controller and the screen   

    Button _decPageButton = Button(DEC_PAGE_BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);                   // button for load the previous page of MIDI messages into the MIDI components
    Button _incPageButton = Button(INC_PAGE_BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);                   // button for load the next page of MIDI messages into the MIDI components
    Button _editButton = Button(EDIT_MODE_BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);                     // button that activates/deactivates the edit mode.
    Button _operationModeButton = Button(OPERATION_MODE_BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);       // button for change the operation mode: Controller or Sequencer
    
    Potentiometer _selectValuePot = Potentiometer(VALUE_POT_PIN, WINDOW_SIZE);                          // potentiometer for set the tempo in BPM of the controller.
    Led _midiLed = Led(MIDI_TRANSMISSION_PIN);                                                           // Led that blinks when MIDI information is being sent
    Button _multiplePurposeButton = Button(MULTIPLE_PURPOSE_BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);   // Button that activates/deactivates MIDI clock signal sending or move the cursor to the next value to edit
    uint16_t _bpm;                                                                                      // Current MIDI controller tempo in BPMs.   
    uint32_t _lastTimeBlink;                                                                                 // Led blink timer
    uint32_t _lastTimeBlinkFade;                                                                                 // Led blink fade timer
    uint32_t _lastTimeClock;                                                                            // MIDI clock tick timer  
    uint8_t _isMIDIClockOn;                                                                             // set to TRUE when controller is sending MIDI Clock Data. FALSE otherwise

    MidiWorker * _midiWorker;                                                                           // object to manage the MIDI functionality
    
    Sequencer _sequencer = Sequencer(Sequencer::FORWARD, Sequencer::QUARTER, &_memoryManager, &_screenManager);

    SyncManager _syncManager = SyncManager(BAR_LENGTH);
    uint8_t _waitForStart;
    uint8_t _waitForLoadSequence;
    
    enum State {CONTROLLER, SEQUENCER, EDIT_PAGE, EDIT_GLOBAL_CONFIG};                                             // Controller status list
    enum SubState {MIDI_CLOCK_ON, MIDI_CLOCK_OFF, EDIT_GLOBAL_MODE, EDIT_GLOBAL_ROOT_NOTE, EDIT_GLOBAL_MIDI_CH, 
                  DEFAULT_EDIT_MSG, EDIT_MIDI_TYPE, EDIT_NOTE, EDIT_VELOCITY, EDIT_CC, PLAYBACK_ON, PLAYBACK_OFF}; // Controller substatus list
    char _state, _subState;                                                                                        // Controller current status and substatus

    GlobalConfig _globalConfig = GlobalConfig();                                                        // Object containing the global configuration

    void processMidiComponent(IMIDIComponent * component);
   
    void printSerial(MIDIMessage message);
    void savePage(uint8_t page);
    void loadPage(uint8_t page);
    void updateMIDIClockState();
    void updateSequencerPlayBackStatus();
    void moveCursorToValue();
    void moveCursorToGLobalConfigParameter();
};
#endif
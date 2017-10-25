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
#include <MIDIButton.h>
#include <MIDIPotentiometer.h> 
#include <ControllerConfig.h>
#include <Led.h>

typedef midi::MidiInterface<HardwareSerial> MidiInterface;

class MIDIController
{
  public:   
    MIDIController(MidiInterface& inInterface, IMIDIComponent ** components, uint8_t numMIDIComponents);
    MIDIController(IMIDIComponent ** components, uint8_t numMIDIComponents);
      
    void begin(); 
    void processMIDIComponents();
 
    /*void processShiftButton(MIDIButton * midiButtons, MIDIPotentiometer * midiPots);
    uint8_t wasShiftButtonReleased();
    uint8_t isShiftMode();*/

  private:
    uint8_t _numMIDIComponents;
    IMIDIComponent ** _midiComponents;

    /*Button _shiftButton = Button(SHIFT_BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);
    Led _shiftButtonLed1 = Led(SHIFT_BUTTON_LED_PIN_1, HIGH);
    Led _shiftButtonLed2 = Led(SHIFT_BUTTON_LED_PIN_2, LOW);
    uint8_t _shiftMode = 0;*/
    
    MidiInterface& _mMidi;

    void processMidiComponent(IMIDIComponent * component);
    void sendMIDIMessage(MIDIMessage * message);
    void printSerial(MIDIMessage message);
};
#endif
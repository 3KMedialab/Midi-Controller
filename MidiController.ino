/*
 * MIDI Controller
 *
 * Create the components that will be part of the Controller.
 * Process the components of the controller.
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

#include <Pitches.h>
#include <MIDIController.h>
  
// Create the MIDI interface object
MidiInterface MIDI(Serial);

//-------------------------------- M I D I  B U T T O N S  S E C T I O N ---------------------------------------------
// MIDI BUTTON 1 MESSAGES
MIDIMessage onPressedMessageB1(midi::NoteOn, NOTE_C3, 127, 1);
MIDIMessage onReleasedMessageB1(midi::NoteOff, NOTE_C3, 127, 1);

// MIDI BUTTON 2 MESSAGES
MIDIMessage onPressedMessageB2(midi::NoteOn, NOTE_F3, 127, 1);
MIDIMessage onReleasedMessageB2(midi::NoteOff, NOTE_F3, 127, 1);

// MIDI BUTTON 3 MESSAGES
MIDIMessage onPressedMessageB3(midi::NoteOn, NOTE_G3, 127, 1);
MIDIMessage onReleasedMessageB3(midi::NoteOff, NOTE_G3, 127, 1);

MIDIButton b1(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS, &onPressedMessageB1, &onReleasedMessageB1);
MIDIButton b2(MIDI_BUTTON_PIN2, PULLUP, INVERT, DEBOUNCE_MS, &onPressedMessageB2, &onReleasedMessageB2);
MIDIButton b3(MIDI_BUTTON_PIN3, PULLUP, INVERT, DEBOUNCE_MS, &onPressedMessageB3, &onReleasedMessageB3);
//-------------------------------- E N D  M I D I  B U T T O N S  S E C T I O N ---------------------------------------------

//-------------------------------- M I D I  P O T E N T I O M E T E R S  S E C T I O N ---------------------------------------------
// MIDI POTENTIOMETER 1 MESSAGE
MIDIMessage midiMessageP1(midi::ControlChange, midi::BreathController, 0, 1);

MIDIPotentiometer p1(MIDI_POT_PIN1, WINDOW_SIZE, &midiMessageP1);

//-------------------------------- E N D  M I D I  P O T E N T I O M E T E R S  S E C T I O N ---------------------------------------------

IMIDIComponent * components [NUM_MIDI_BUTTONS+NUM_MIDI_POTS] = {&b1, &b2, &b3, &p1};

// Creates the MIDI Controller object
MIDIController controller(MIDI, components, NUM_MIDI_BUTTONS+NUM_MIDI_POTS);
 
 void setup(void)
 {
   //Initializes MIDI interface
    controller.begin();
 }
 
 void loop(void)
 {
    // Process the MIDI components 
    controller.processMIDIComponents();
    
    // Process the shift button
    /*if (controller.wasShiftButtonReleased())
    {
        if (!controller.isShiftMode())
        {
            controller.processShiftButton(shiftMidiButtons, shiftMidiPots);    
        }
        else
        {
            controller.processShiftButton(midiButtons, midiPots);            
        }      
     }*/
 }
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
#include <MIDIButton.h>
#include <MIDIPotentiometer.h>
  
// Create the MIDI interface object
MidiInterface MIDI(Serial);

//-------------------------------- M I D I  B U T T O N S  S E C T I O N ---------------------------------------------
// MIDI BUTTONS
MIDIButton b1(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);
/*MIDIButton b2(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton b3(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton b4(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton b5(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton b6(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton b7(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton b8(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton b9(MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);*/
//-------------------------------- E N D  M I D I  B U T T O N S  S E C T I O N ---------------------------------------------

//-------------------------------- M I D I  P O T E N T I O M E T E R S  S E C T I O N ---------------------------------------------
// MIDI POTENTIOMETERS
MIDIPotentiometer p1(MIDI_POT_PIN1, WINDOW_SIZE);
/*MIDIPotentiometer p2(MIDI_POT_PIN1, WINDOW_SIZE);
MIDIPotentiometer p3(MIDI_POT_PIN1, WINDOW_SIZE);*/
//-------------------------------- E N D  M I D I  P O T E N T I O M E T E R S  S E C T I O N ---------------------------------------------

//IMIDIComponent * components [NUM_MIDI_BUTTONS+NUM_MIDI_POTS] = {&b1, &b2, &b3,&b4,&b5,&b6,&b7,&b8,&b9,&p1,&p2,&p3};
IMIDIComponent * components [NUM_MIDI_BUTTONS+NUM_MIDI_POTS] = {&b1,&p1};

// Creates the MIDI Controller object
MIDIController controller(MIDI, components, NUM_MIDI_BUTTONS+NUM_MIDI_POTS);
 
 void setup(void)
 {
   //Initializes MIDI interface
    controller.begin();
 }
 
 void loop(void)
  {
    // Process MIDI clock components
    controller.processMIDIClockComponents();
    
    // Process the select value potentiometer
    controller.processSelectValuePot();

    // Process MIDI clock components
    controller.processMIDIClockComponents();
    
    // Process the MIDI components 
    controller.processMIDIComponents();

    // Process MIDI clock components
    controller.processMIDIClockComponents();
    
    // Process the page inc/dec buttons
    controller.processIncDecButtons();

    // Process MIDI clock components
    controller.processMIDIClockComponents();

    // Process set edit mode on/off button
    controller.processEditModeButton();
  }
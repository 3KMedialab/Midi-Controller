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
#include <MIDIButton.cpp>
#include <MIDIPotentiometer.h>
#include <MIDIPotentiometer.cpp>
#include <Multiplexer.h>
#include <MuxButton.h>
#include <MuxPotentiometer.h>
  
// Create the MIDI interface object
MidiInterface MIDI(Serial);

// Mux for MIDI Buttons (digital)
Multiplexer muxMIDIButtons1 (MUX1_MIDI_BUTTONS_OUTPUT_PIN, MUX1_MIDI_BUTTONS_NUM_CONTROL_PINS, MUX1_MIDI_BUTTONS_CONTROL_PINS, ComponentType::INPUT_DIGITAL, PULLUP);

// Mux for MIDI Potentiometers (analog)
Multiplexer muxMIDIPots1 (MUX1_MIDI_POTS_OUTPUT_PIN, MUX1_MIDI_POTS_NUM_CONTROL_PINS, MUX1_MIDI_POTS_CONTROL_PINS, ComponentType::INPUT_ANALOG);

//-------------------------------- M I D I  B U T T O N S  S E C T I O N ---------------------------------------------
// MIDI BUTTONS directly connected to Arduino board
MIDIButton<Button> b1(MIDI_BUTTON1_PIN, PULLUP, INVERT, DEBOUNCE_MS);

/*
MIDIButton<Button> b1(MIDI_BUTTON1_PIN, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton<Button> b2(MIDI_BUTTON2_PIN, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton<Button> b3(MIDI_BUTTON3_PIN, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton<Button> b4(MIDI_BUTTON4_PIN, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton<Button> b5(MIDI_BUTTON5_PIN, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton<Button> b6(MIDI_BUTTON6_PIN, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton<Button> b7(MIDI_BUTTON7_PIN, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton<Button> b8(MIDI_BUTTON8_PIN, PULLUP, INVERT, DEBOUNCE_MS);
MIDIButton<Button> b9(MIDI_BUTTON9_PIN, PULLUP, INVERT, DEBOUNCE_MS);
*/

 

// MIDI Buttons connected to Arduino board through multiplexer
//MIDIButton<MuxButton> b1(&muxMIDIButtons1, MIDI_BUTTON1_MUX1_CHANNEL, INVERT, DEBOUNCE_MS);
MIDIButton<MuxButton> b2(&muxMIDIButtons1, MIDI_BUTTON1_MUX1_CHANNEL, INVERT, DEBOUNCE_MS);
MIDIButton<MuxButton> b3(&muxMIDIButtons1, MIDI_BUTTON2_MUX1_CHANNEL, INVERT, DEBOUNCE_MS);
MIDIButton<MuxButton> b4(&muxMIDIButtons1, MIDI_BUTTON3_MUX1_CHANNEL, INVERT, DEBOUNCE_MS);
MIDIButton<MuxButton> b5(&muxMIDIButtons1, MIDI_BUTTON4_MUX1_CHANNEL, INVERT, DEBOUNCE_MS);
MIDIButton<MuxButton> b6(&muxMIDIButtons1, MIDI_BUTTON5_MUX1_CHANNEL, INVERT, DEBOUNCE_MS);
MIDIButton<MuxButton> b7(&muxMIDIButtons1, MIDI_BUTTON6_MUX1_CHANNEL, INVERT, DEBOUNCE_MS);
MIDIButton<MuxButton> b8(&muxMIDIButtons1, MIDI_BUTTON7_MUX1_CHANNEL, INVERT, DEBOUNCE_MS);
MIDIButton<MuxButton> b9(&muxMIDIButtons1, MIDI_BUTTON8_MUX1_CHANNEL, INVERT, DEBOUNCE_MS);

//-------------------------------- E N D  M I D I  B U T T O N S  S E C T I O N ---------------------------------------------

//-------------------------------- M I D I  P O T E N T I O M E T E R S  S E C T I O N ---------------------------------------------
// MIDI Potentiometers directly connected to Arduino board
/*MIDIPotentiometer<Potentiometer> p1(MIDI_POT1_PIN, WINDOW_SIZE);
MIDIPotentiometer<Potentiometer> p2(MIDI_POT2_PIN, WINDOW_SIZE);
MIDIPotentiometer<Potentiometer> p3(MIDI_POT3_PIN, WINDOW_SIZE);*/


// MIDI Potentiometers connected to Arduino board through multiplexer
MIDIPotentiometer<MuxPotentiometer> p1(&muxMIDIPots1, MIDI_POT1_MUX1_CHANNEL, WINDOW_SIZE);
MIDIPotentiometer<MuxPotentiometer> p2(&muxMIDIPots1, MIDI_POT2_MUX1_CHANNEL, WINDOW_SIZE);
MIDIPotentiometer<MuxPotentiometer> p3(&muxMIDIPots1, MIDI_POT3_MUX1_CHANNEL, WINDOW_SIZE);

//-------------------------------- E N D  M I D I  P O T E N T I O M E T E R S  S E C T I O N ---------------------------------------------

// MIDI components assigned to the controller
IMIDIComponent * components [NUM_MIDI_BUTTONS+NUM_MIDI_POTS] = {&b1,&b2,&b3,&b4,&b5,&b6,&b7,&b8,&b9,&p1,&p2,&p3};

// Creates the MIDI Controller object
MIDIController controller(MIDI, components, NUM_MIDI_BUTTONS+NUM_MIDI_POTS);
 
 void setup(void)
 {
   //Initializes MIDI interface
    controller.begin();
 }
 
 void loop(void)
  {
    // Process multiple purpose button for activate/deactivate MIDI clock signal or move to the next value to edit
    controller.processMultiplePurposeButton();

    // Send MIDI clock
    controller.sendMIDIClock();
    
    // Process the select value potentiometer
    controller.processSelectValuePot();

    // Send MIDI clock
    controller.sendMIDIClock();
    
    // Process the MIDI components 
    controller.processMIDIComponents();

    // Send MIDI clock
    controller.sendMIDIClock();
    
    // Process the page inc/dec buttons
    controller.processIncDecButtons();

    // Send MIDI clock
    controller.sendMIDIClock();

    // Process set edit mode on/off button
    controller.processEditModeButton();
  }
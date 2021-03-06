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
#include <MidiWorker.h>
#include <MIDIButton.h>
#include <MIDIButton.cpp>
#include <MIDIPotentiometer.h>
#include <MIDIPotentiometer.cpp>
#include <Multiplexer.h>
#include <MuxButton.h>
#include <MuxPotentiometer.h>
#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include <TimerOne.h>

#define MICROSECONDS_PER_MINUTE 60000000

// Create the MIDI interface object
MidiInterface MIDI(Serial);

// Mux for MIDI Buttons (digital)
Multiplexer muxMIDIButtons1(MUX1_MIDI_BUTTONS_OUTPUT_PIN, MUX1_MIDI_BUTTONS_NUM_CONTROL_PINS, MUX1_MIDI_BUTTONS_CONTROL_PINS, ComponentType::INPUT_DIGITAL, PULLUP);

// Mux for MIDI Potentiometers (analog)
//Multiplexer muxMIDIPots1 (MUX1_MIDI_POTS_OUTPUT_PIN, MUX1_MIDI_POTS_NUM_CONTROL_PINS, MUX1_MIDI_POTS_CONTROL_PINS, ComponentType::INPUT_ANALOG);

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
MIDIPotentiometer<Potentiometer> p1(MIDI_POT1_PIN, WINDOW_SIZE);
MIDIPotentiometer<Potentiometer> p2(MIDI_POT2_PIN, WINDOW_SIZE);
MIDIPotentiometer<Potentiometer> p3(MIDI_POT3_PIN, WINDOW_SIZE);

// MIDI Potentiometers connected to Arduino board through multiplexer
/*MIDIPotentiometer<MuxPotentiometer> p1(&muxMIDIPots1, MIDI_POT1_MUX1_CHANNEL, WINDOW_SIZE);
MIDIPotentiometer<MuxPotentiometer> p2(&muxMIDIPots1, MIDI_POT2_MUX1_CHANNEL, WINDOW_SIZE);
MIDIPotentiometer<MuxPotentiometer> p3(&muxMIDIPots1, MIDI_POT3_MUX1_CHANNEL, WINDOW_SIZE);*/

//-------------------------------- E N D  M I D I  P O T E N T I O M E T E R S  S E C T I O N ---------------------------------------------

// MIDI components assigned to the controller
IMIDIComponent *components[NUM_MIDI_BUTTONS + NUM_MIDI_POTS] = {&b1, &b2, &b3, &b4, &b5, &b6, &b7, &b8, &b9, &p1, &p2, &p3};

// MIDI processing handler
MidiWorker worker(MIDI);

// Creates the MIDI Controller object
volatile MIDIController controller(&worker, components, NUM_MIDI_BUTTONS + NUM_MIDI_POTS);

// Times a MIDI tick is sent. Used to calculate when next step should be played within a sequence
volatile uint8_t periods = 0;

void setup(void)
{
  //Initializes MIDI interface
  worker.begin();
  
  controller.begin();

  // Used for random step playback mode
  randomSeed(analogRead(0));

  // Default time a MIDI tick is sent
  Timer1.initialize((MICROSECONDS_PER_MINUTE / controller.getBpm()) / 24);
  
  // TODO: move timer1 logic to SyncManager class
  Timer1.attachInterrupt(executeRealTimeTasks);
}

/******************************************************************************/
/* INTERRUPT METHOD LINKED TO TIMER1                                          */
/* Executes the real time tasks such as send MIDI clock and sequencer playback*/
/******************************************************************************/
void executeRealTimeTasks()
{
  // update the period in case the Bpm has changed
  Timer1.setPeriod((MICROSECONDS_PER_MINUTE / controller.getBpm()) / 24);
  periods++;

  // start sequence
  if (controller.getResetMIDIClockPeriod())
  {
    // reset the period of the sequence in order to start in sync with the MIDI clock
    switch (controller.getStepSize())
    {
    case 1:
      periods = 24;
      break;

    case 2:
      periods = 12;
      break;

    case 4:
      periods = 6;
      break;

    case 8:
      periods = 3;
      break;
    }

    controller.setResetMIDIClockPeriod(0);
    Timer1.restart();
  }

  // play next step of the sequence regarding the step size
  if ((controller.getStepSize() == 1 && periods == 24) ||
      (controller.getStepSize() == 2 && periods == 12) ||
      (controller.getStepSize() == 4 && periods == 6) ||
      (controller.getStepSize() == 8 && periods == 3))
  {
    periods = 0;
    controller.playBackSequence();
  }

  controller.sendMIDIClock();  
}

void loop(void)
{

  // Process the select value potentiometer
  controller.processSelectValuePot();

  // Process the MIDI components
  controller.processMIDIComponents();

  // Process multiple purpose button for activate/deactivate MIDI clock signal or move to the next value to edit
  controller.processMultiplePurposeButton();

  // Process the page inc/dec buttons
  controller.processIncDecButtons();

  // Process set edit mode on/off button
  controller.processEditModeButton();

  // Process change operation mode button
  controller.processOperationModeButton();
}
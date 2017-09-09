/*
 * MIDI Controller
 *
 * Create the components that will be part of the Controller.
 * Process the components of the controller.
 *
 * Created by Daniel Segura, 01/09/2017
 */

#include <Pitches.h>
#include <MIDIController.h>
  
// Create the MIDI interface object
MidiInterface MIDI(Serial);

//-------------------------------- B U T T O N S  S E C T I O N ---------------------------------------------
//Shift BUTTON
//Button shiftButton (BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);

//-------------------------------- E N D  B U T T O N S  S E C T I O N ---------------------------------------------


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

// MIDI BUTTONS
MIDIButton midiButtons [NUM_MIDI_BUTTONS] = { 
    {MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS, &onPressedMessageB1, &onReleasedMessageB1},
    {MIDI_BUTTON_PIN2, PULLUP, INVERT, DEBOUNCE_MS, &onPressedMessageB2, &onReleasedMessageB2},
    {MIDI_BUTTON_PIN3, PULLUP, INVERT, DEBOUNCE_MS, &onPressedMessageB3, &onReleasedMessageB3} 
};

// MIDI BUTTON 1 SHIFT MESSAGES
MIDIMessage onPressedShiftMessageB1(midi::NoteOn, NOTE_C4, 127, 1);
MIDIMessage onReleasedShiftMessageB1(-1, -1, -1, -1);

// MIDI BUTTON 2 SHIFT MESSAGES
MIDIMessage onPressedShiftMessageB2(midi::NoteOn, NOTE_F4, 127, 1);
MIDIMessage onReleasedShiftMessageB2(-1, -1, -1, -1);

// MIDI BUTTON 3 SHIFT MESSAGES
MIDIMessage onPressedShiftMessageB3(midi::ControlChange, midi::Sustain, 127, 1);
MIDIMessage onReleasedShiftMessageB3(midi::ControlChange, midi::Sustain, 0, 1);

// MIDI SHIFT BUTTONS
MIDIButton shiftMidiButtons [NUM_MIDI_BUTTONS] = { 
    {MIDI_BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS, &onPressedShiftMessageB1, &onReleasedShiftMessageB1},
    {MIDI_BUTTON_PIN2, PULLUP, INVERT, DEBOUNCE_MS, &onPressedShiftMessageB2, &onReleasedShiftMessageB2},
    {MIDI_BUTTON_PIN3, PULLUP, INVERT, DEBOUNCE_MS, &onPressedShiftMessageB3, &onReleasedShiftMessageB3}
};


//-------------------------------- E N D  M I D I  B U T T O N S  S E C T I O N ---------------------------------------------

//-------------------------------- M I D I  P O T E N T I O M E T E R S  S E C T I O N ---------------------------------------------
// MIDI POTENTIOMETER 1 MESSAGE
MIDIMessage midiMessageP1(midi::ControlChange, midi::BreathController, 0, 1);

// MIDI POTENTIOMETERS
MIDIPotentiometer midiPots [NUM_MIDI_POTS] = {
    //{MIDI_POT_PIN1, WINDOW_SIZE, &midiMessageP1}
};

// MIDI POTENTIOMETER 1 SHIFT MESSAGE
MIDIMessage midiShiftMessageP1(midi::ControlChange, midi::ModulationWheel, 0, 1);

// MIDI SHIFT POTENTIOMETERS
MIDIPotentiometer shiftMidiPots [NUM_MIDI_POTS] = {
    //{MIDI_POT_PIN1, WINDOW_SIZE, &midiShiftMessageP1}
};
//-------------------------------- E N D  M I D I  P O T E N T I O M E T E R S  S E C T I O N ---------------------------------------------

// Creates the MIDI Controller object
MIDIController controller(MIDI, midiButtons, midiPots, NUM_MIDI_BUTTONS, NUM_MIDI_POTS);
 
 void setup(void)
 {
   controller.begin();
 }
 
 void loop(void)
 {
     controller.processMIDIButtons();
     controller.processMIDIPots(); 
     
     if (controller.wasShiftButtonReleased())
     {
        if (!controller.isShiftMode())
        {
            controller.processShiftButton(shiftMidiButtons, shiftMidiPots);    
        }
        else
        {
            controller.processShiftButton(midiButtons, midiPots);            
        }      
     }
 }
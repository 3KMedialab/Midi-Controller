/*----------------------------------------------------------------------*
 * Example sketch for Arduino Button Library by Jack Christensen        *
 *                                                                      *
 * The simplest example. Using a tactile button switch to turn          *
 * the Arduino's pin 13 LED on and off. Wire a switch from Arduino      *
 * pin 2 to ground.                                                     *
 *                                                                      *
 * This work is licensed under the Creative Commons Attribution-        *
 * ShareAlike 3.0 Unported License. To view a copy of this license,     *
 * visit http://creativecommons.org/licenses/by-sa/3.0/ or send a       *
 * letter to Creative Commons, 171 Second Street, Suite 300,            *
 * San Francisco, California, 94105, USA.                               *
 *----------------------------------------------------------------------*/

 #include <MIDIController.h>
  
 #define BUTTON_PIN1 2
 #define BUTTON_PIN2 3
 #define BUTTON_PIN3 4   //Connect a tactile button switch (or something similar)
                            //from Arduino pin 2 to ground.
 #define PULLUP true        //To keep things simple, we use the Arduino's internal pullup resistor.
 #define INVERT true        //Since the pullup resistor will keep the pin high unless the
                            //switch is closed, this is negative logic, i.e. a high state
                            //means the button is NOT pressed. (Assuming a normally open switch.)
 #define DEBOUNCE_MS 20     //A debounce time of 20 milliseconds usually works well for tactile button switches.
 #define LED_PIN 13         //The standard Arduino "Pin 13" LED
 
 MIDI_CREATE_DEFAULT_INSTANCE();
 
 Button myBtn1(BUTTON_PIN1, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button
 Button myBtn2(BUTTON_PIN2, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button
 
 MIDIMessage noteOnOffMsgs1[2] ={ {midi::NoteOn, NOTE_A4, 127, 1}, {midi::NoteOff, NOTE_A4, 127, 1} };
 MIDIMessage shiftMessages1[2] ={ {midi::NoteOn, NOTE_C3, 127, 1}, {midi::NoteOff, NOTE_C3, 127, 1} };
 MIDIButton midiButton(BUTTON_PIN3, PULLUP, INVERT, DEBOUNCE_MS, noteOnOffMsgs1, shiftMessages1);    //Declare the button
 
 MIDIMessage ccMessage(midi::ControlChange, midi::BreathController, 0, 1);
 MIDIPotentiometer midiPot(A0, &ccMessage);
 uint16_t controlValue = 0;

 Potentiometer myPot(A0);
 
 boolean ledState;          //A variable that keeps the current LED status
 
 void setup(void)
 {
    MIDI.begin(); 
    pinMode(LED_PIN, OUTPUT);    //Set the LED pin as an output
 }
 
 void loop(void)
 {
     myBtn1.read();                    //Read the button
 
     if (myBtn1.wasReleased()) {       //If the button was released, change the LED state
         ledState = !ledState;
         digitalWrite(LED_PIN, ledState);       
     }
 
     myBtn2.read();                    //Read the button
 
     if (myBtn2.wasReleased()) {       //If the button was released, change the LED state
         ledState = !ledState;
         digitalWrite(LED_PIN, ledState);        
     }
 
     midiButton.read();                    //Read the button
 
     if (midiButton.wasPressed()) {       //If the button was released, change the LED state
       MIDI.sendNoteOn(midiButton.getMessage(0).getDataByte1(), midiButton.getMessage(0).getDataByte2(), midiButton.getMessage(0).getChannel() );
     }    
     
     if (midiButton.wasReleased()) {       //If the button was released, change the LED state
       MIDI.sendNoteOff(midiButton.getMessage(1).getDataByte1(), midiButton.getMessage(1).getDataByte2(), midiButton.getMessage(1).getChannel() );
     }
 
     // PROCESS KNOBS
     controlValue = midiPot.getSmoothValue();
 
     if (midiPot.wasChanged()) {
          MIDI.sendControlChange(midiPot.getMessage().getDataByte1(), map(controlValue,0,1023,0,127), midiPot.getMessage().getChannel());
     }
 }
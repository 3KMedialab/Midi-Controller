
//-------------------------------- B U T T O N S  S E C T I O N ---------------------------------------------

/*************************************************
 * BUTTONS CONFIGURATION
 *************************************************/
 
 //TRUE: use the Arduino's internal pullup resistor.
 const uint8_t PULLUP = true;
 
 //Since the pullup resistor will keep the pin high unless the switch is closed, this is negative logic, i.e. a high state
 //means the button is NOT pressed. (Assuming a normally open switch.)
 // Use TRUE when using Arduino' pullup resistor
 const uint8_t INVERT = true;  

 //Debounce time for the buttons. A time of 20 milliseconds usually works well for tactile button switches.                            
 const uint8_t DEBOUNCE_MS = 20;     

 /*************************************************
 * PAGE BUTTONS CONFIG
 *************************************************/

 // Input Digital Pins for page buttons
 const uint8_t DEC_PAGE_BUTTON_PIN = 12;
 const uint8_t INC_PAGE_BUTTON_PIN = 3;

/*************************************************
 * MIDI BUTTONS CONFIGURATION
 *************************************************/
 
 // Number of MIDI Buttons to add to the controller
 const uint8_t NUM_MIDI_BUTTONS = 2;

 // Input Digital Pin for each MIDI button
 const uint8_t MIDI_BUTTON_PIN1 = 2;
 const uint8_t MIDI_BUTTON_PIN2 = 4;

 //-------------------------------- E N D  B U T T O N S  S E C T I O N ---------------------------------------------

 //-------------------------------- P O T E N T I O M E T E R S   S E C T I O N ---------------------------------------------

 /*************************************************
 * POTENTIOMETERS CONFIGURATION
 *************************************************/

 // Input Analog Pin for each Potentiometer
 const uint8_t POT_PIN1 = A5;

 // Number of measures to smooth the read analog
 const uint8_t WINDOW_SIZE = 10;

/*************************************************
 * MIDI POTENTIOMETERS CONFIGURATION
 *************************************************/

 // Number of MIDI Buttons to add to the controller
 const uint8_t NUM_MIDI_POTS = 1;
  
 // Input Analog Pin for each MIDI potentiometer
 const uint8_t MIDI_POT_PIN1 = A0;

  //-------------------------------- E N D  P O T E N T I O M E T E R S   S E C T I O N ---------------------------------------------
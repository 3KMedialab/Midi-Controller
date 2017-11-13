
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

 // Time limit (ms) for detecting Pressed For events
 const uint32_t PRESSED_FOR_WAIT = 2000;

 /*************************************************
 * PAGE BUTTONS CONFIG
 *************************************************/

 // Input Digital Pins for page buttons
 const uint8_t DEC_PAGE_BUTTON_PIN = 12;
 const uint8_t INC_PAGE_BUTTON_PIN = 3;

 // Input Digital Pin for send MIDI clock button
 const uint8_t MIDI_CLOCK_BUTTON_PIN = 8;

 // Input Digital Pin for edit mode button
 const uint8_t EDIT_MODE_BUTTON_PIN = 4;

/*************************************************
 * MIDI BUTTONS CONFIGURATION
 *************************************************/
 
 // Number of MIDI Buttons to add to the controller
 const uint8_t NUM_MIDI_BUTTONS = 1;

 // Input Digital Pin for each MIDI button
 const uint8_t MIDI_BUTTON_PIN1 = 2;
 /*const uint8_t MIDI_BUTTON_PIN2 = 2;
 const uint8_t MIDI_BUTTON_PIN3 = 2;
 const uint8_t MIDI_BUTTON_PIN4 = 2;
 const uint8_t MIDI_BUTTON_PIN5 = 2;
 const uint8_t MIDI_BUTTON_PIN6 = 2;
 const uint8_t MIDI_BUTTON_PIN7 = 2;
 const uint8_t MIDI_BUTTON_PIN8 = 2;
 const uint8_t MIDI_BUTTON_PIN9 = 2;*/

 //-------------------------------- E N D  B U T T O N S  S E C T I O N ---------------------------------------------

 //-------------------------------- P O T E N T I O M E T E R S   S E C T I O N ---------------------------------------------

 /*************************************************
 * POTENTIOMETERS CONFIGURATION
 *************************************************/

 // Number of measures to smooth the read analog
 const uint8_t WINDOW_SIZE = 10;
 
 // Input Analog Pin for each Potentiometer
 const uint8_t VALUE_POT_PIN = A1;

/*************************************************
 * MIDI POTENTIOMETERS CONFIGURATION
 *************************************************/

 // Number of MIDI Buttons to add to the controller
 const uint8_t NUM_MIDI_POTS = 1;
  
 // Input Analog Pin for each MIDI potentiometer
 const uint8_t MIDI_POT_PIN1 = A0;
 /*const uint8_t MIDI_POT_PIN2 = A0;
 const uint8_t MIDI_POT_PIN3 = A0;*/

//-------------------------------- E N D  P O T E N T I O M E T E R S   S E C T I O N ---------------------------------------------

//-------------------------------- L E D  S E C T I O N ---------------------------------------------------------
  const uint8_t BPM_LED_PIN = 13;
//-------------------------------- E N D  O F  L E D  S E C T I O N ---------------------------------------------

  //-------------------------------- S C R E E N  S E C T I O N ---------------------------------------------------------
  const uint8_t I2C_ADDRESS = 0x27;
  const uint8_t ROW_LENGTH = 16;
  const uint8_t ROWS = 2;
  //-------------------------------- E N D  O F  S C R E E N  S E C T I O N ---------------------------------------------

    //-------------------------------- T E M P O  S E C T I O N ---------------------------------------------------------
    const uint8_t MIN_BPM = 10;
    const uint16_t MAX_BPM = 300;
    //-------------------------------- E N D  O F  T E M P O  S E C T I O N ---------------------------------------------
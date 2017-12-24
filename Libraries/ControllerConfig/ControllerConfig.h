#ifndef ControllerConfig_h
#define ControllerConfig_h
//-------------------------------- B U T T O N S  S E C T I O N ---------------------------------------------

/*************************************************
 * GLOBAL BUTTON CONFIGURATION
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
 const uint8_t DEC_PAGE_BUTTON_PIN = 6;
 const uint8_t INC_PAGE_BUTTON_PIN = 5;

 // Input Digital Pin for multiple purpose button
 const uint8_t MULTIPLE_PURPOSE_BUTTON_PIN = 8;

 // Input Digital Pin for edit mode button
 const uint8_t EDIT_MODE_BUTTON_PIN = 7;

/*************************************************
 * MIDI BUTTONS CONFIGURATION
 *************************************************/
 
 // Number of MIDI Buttons to add to the controller
 const uint8_t NUM_MIDI_BUTTONS = 9;

 // Input Digital Pin for each MIDI button connected directly to Arduino
 const uint8_t MIDI_BUTTON9_PIN = 5;

 /*
 const uint8_t MIDI_BUTTON1_PIN = 3;
 const uint8_t MIDI_BUTTON2_PIN = 4;
 const uint8_t MIDI_BUTTON3_PIN = 5;
 const uint8_t MIDI_BUTTON4_PIN = 6;
 const uint8_t MIDI_BUTTON5_PIN = 7;
 const uint8_t MIDI_BUTTON6_PIN = 8;
 const uint8_t MIDI_BUTTON7_PIN = 9;
 const uint8_t MIDI_BUTTON8_PIN = 10;
 const uint8_t MIDI_BUTTON9_PIN = 11
 */

 // Channel of multiplexer where the MIDI Buttons are connected
 const uint8_t MIDI_BUTTON1_MUX1_CHANNEL = 0;
 const uint8_t MIDI_BUTTON2_MUX1_CHANNEL = 1;
 const uint8_t MIDI_BUTTON3_MUX1_CHANNEL = 2;
 const uint8_t MIDI_BUTTON4_MUX1_CHANNEL = 3;
 const uint8_t MIDI_BUTTON5_MUX1_CHANNEL = 4;
 const uint8_t MIDI_BUTTON6_MUX1_CHANNEL = 5;
 const uint8_t MIDI_BUTTON7_MUX1_CHANNEL = 6;
 const uint8_t MIDI_BUTTON1_MUX2_CHANNEL = 0;
 const uint8_t MIDI_BUTTON2_MUX2_CHANNEL = 1;

 //-------------------------------- E N D  B U T T O N S  S E C T I O N ---------------------------------------------

 //-------------------------------- P O T E N T I O M E T E R S   S E C T I O N ---------------------------------------------

 /*************************************************
 * POTENTIOMETERS CONFIGURATION
 *************************************************/

 // Number of measures to smooth the read analog
 const uint8_t WINDOW_SIZE = 5;
 
 // Input Analog Pin for each Potentiometer
 const uint8_t VALUE_POT_PIN = A1;

/*************************************************
 * MIDI POTENTIOMETERS CONFIGURATION
 *************************************************/

 // Number of MIDI Potentiometers to add to the controller
 const uint8_t NUM_MIDI_POTS = 3;
  
 // Input Analog Pin for each MIDI potentiometer connected directly to Arduino
 /*const uint8_t MIDI_POT1_PIN = A0;
 const uint8_t MIDI_POT2_PIN = A2;
 const uint8_t MIDI_POT3_PIN = A0; */

 // Channel of multiplexer where the MIDI potentiometers are connected
 const uint8_t MIDI_POT1_MUX1_CHANNEL = 0;
 const uint8_t MIDI_POT2_MUX1_CHANNEL = 1;
 const uint8_t MIDI_POT3_MUX1_CHANNEL = 2;

//-------------------------------- E N D  P O T E N T I O M E T E R S   S E C T I O N ---------------------------------------------

//-------------------------------- L E D  S E C T I O N ---------------------------------------------------------
  const uint8_t MIDI_TRANSMISSION_PIN = 9;
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

//-------------------------------- M U L T I P L E X E R  S E C T I O N  ---------------------------------------------------------
const uint8_t MUX1_MIDI_BUTTONS_OUTPUT_PIN = 11;
const uint8_t MUX1_MIDI_BUTTONS_NUM_CONTROL_PINS = 3;
const uint8_t MUX1_MIDI_BUTTONS_CONTROL_PINS [MUX1_MIDI_BUTTONS_NUM_CONTROL_PINS] = {2, 3, 4};

const uint8_t MUX2_MIDI_BUTTONS_OUTPUT_PIN = 12;
const uint8_t MUX2_MIDI_BUTTONS_NUM_CONTROL_PINS = 3;
const uint8_t MUX2_MIDI_BUTTONS_CONTROL_PINS [MUX2_MIDI_BUTTONS_NUM_CONTROL_PINS] = {2, 3, 4};

const uint8_t MUX1_MIDI_POTS_OUTPUT_PIN = A0;
const uint8_t MUX1_MIDI_POTS_NUM_CONTROL_PINS = 3;
const uint8_t MUX1_MIDI_POTS_CONTROL_PINS [MUX1_MIDI_POTS_NUM_CONTROL_PINS] = {2, 3, 4};

//-------------------------------- E N D  O F  M U L T I P L E X E R  S E C T I O N ---------------------------------------------
#endif
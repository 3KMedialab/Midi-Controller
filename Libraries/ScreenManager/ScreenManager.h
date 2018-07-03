/*
 * ScreenManager.h
 *
 * Class that manage the interactions between the MIDI controller and the screen
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
#ifndef ScreenManager_h
#define ScreenManager_h

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>
#include <MIDIMessage.h>
#include <MIDI.h>
#include <IMIDIComponent.h>
#include <MIDIUtils.h>
#include <GlobalConfig.h>
#include <Step.h>

#define MSG_PAGE 0
#define MSG_TEMPO 1
#define MSG_BPM 2
#define MSG_EDIT 3
#define MSG_CHANNEL 4
#define MSG_NOTE_ON_OFF 5
#define MSG_CTRL_CHANGE 6
#define MSG_CC 7
#define MSG_PGRM_CHANGE 8
#define MSG_PGM 9
#define MSG_VELOCITY 10
#define MSG_SAVED 11
#define MSG_EMPTY_MIDI_TYPE 12
#define MSG_MODE 13
#define MSG_KEY 14
#define MSG_SEQ 15
#define MSG_STEP 16
#define MSG_STEP_LEGATO 17
#define MSG_STEP_ENABLED 18

// Messages that will be displayed on the screen
const char msg_Page[] PROGMEM = "Pg:";
const char msg_Tempo[] PROGMEM = "Tempo: ";
const char msg_Bpm[] PROGMEM = "BPM";
const char msg_Edit[] PROGMEM = {'S','E','L','E','C','T',' ','A','\n','C','O','M','P','O','N','E','N','T','\0'};
const char msg_MsgChannel[] PROGMEM = "Ch:";
const char msg_NoteOnOff[] PROGMEM = "Note On/Off";
const char msg_CtrlChange[] PROGMEM = "Ctrl Change";
const char msg_CC[] PROGMEM = "CC:";
const char msg_PgrmChange[] PROGMEM = "Pgrm Change";
const char msg_PGM[] PROGMEM = "PGM:";
const char msg_Velocity[] PROGMEM = "V:";
const char msg_saved[] PROGMEM = "SAVED OK!";
const char msg_empty_midi_type[] PROGMEM = "No Msg";
const char msg_mode[] PROGMEM = "Mode:";
const char msg_key[] PROGMEM = "Key:";
const char msg_seq[] PROGMEM = "Sq:";
const char msg_step[] PROGMEM = "Step:";
const char msg_step_legato[] PROGMEM = "Leg:";
const char msg_step_enabled[] PROGMEM = "Ena:";

const char * const messages[] PROGMEM = {msg_Page, msg_Tempo, msg_Bpm, msg_Edit, msg_MsgChannel, msg_NoteOnOff, msg_CtrlChange, 
msg_CC, msg_PgrmChange, msg_PGM, msg_Velocity, msg_saved, msg_empty_midi_type, msg_mode, msg_key, msg_seq, msg_step, msg_step_legato, msg_step_enabled};

class ScreenManager
{
  public:   
    void initialize(uint8_t i2cAddress, uint8_t rowLength, uint8_t rows);
    void printDefault(uint8_t page, uint8_t numPages, uint16_t tempo, GlobalConfig globalConf, uint8_t isMidiClockOn);
    void printSelectComponentMessage();
    void printEditGlobalConfig(GlobalConfig globalConf);
    void printSavedMessage();
    void cleanScreen();
    uint8_t isComponentDisplayed();
    void displayPreviousMIDIMsg();
    void displayNextMIDIMsg();
    void displayComponentMIDIMessage(uint8_t msgIndex);
    void setMIDIComponentToDisplay(IMIDIComponent * midiComponent);
    IMIDIComponent * getDisplayedMIDIComponent();  
    uint8_t getDisplayedMessageIndex(); 
    uint8_t getDisplayedMessageType();
    void refreshMIDIData();
    void moveCursorToCC();
    void moveCursorToChannel();
    void moveCursorToMsgType();
    void refreshNoteValue(uint8_t note);
    void moveCursorToNote();
    void moveCursorToRootNote();
    void moveCursorToMIDIChannel();
    void moveCursorToMode();
    void refreshVelocityValue(uint8_t velocity);
    void refreshCCValue(uint8_t cc);
    void refreshChannelValue(uint8_t channel);
    void moveCursorToVelocity();
    void refreshModeData(uint8_t mode);
    void refreshRootNoteData(uint8_t rootNote);
    void refreshMIDIChannelData(uint8_t midiChannel);

    // SEQUENCER METHODS
    void printDefaultSequencer(uint8_t currentSequence, uint8_t totalSequences, uint16_t tempo);
    void updateDisplayedPlaybackStep(Step step, uint8_t sequenceLength, uint8_t currentStep);
    void printEditStepData(Step step, uint8_t currentStep, uint8_t sequenceLength);
    void moveCursorToStepNote();
    void moveCursorToStepLegato();
    void moveCursorToStepEnabled();
    void moveCursorToStepNum();

    uint8_t getDisplayedStepNumber();

  private:
    void getMessage(uint8_t msgIndex, char * buffer);
    void printNoteOnOffMIDIData (MIDIMessage message);
    void printCCMIDIData (MIDIMessage message);
    void printPCMIDIData (MIDIMessage message);
    void printMIDIChannel(uint8_t channel);  
    void clearRangeOnCurentLine(uint8_t row, uint8_t from, uint8_t to);
    String getStepNoteValue (Step step);
    
    IMIDIComponent * _displayedMIDIComponent;                               // MIDI component currently assigned to the screen   
    uint8_t _currentMIDIMessageDisplayed;                                   // MIDI message currently displayed on the screen
    uint8_t _currentDisplayedStep;                                          // Step currently displayed on the screen

    LiquidCrystal_I2C _screen;                                              // LCD screen object

    enum {NOTE_POS=0, VELOCITY_POS=5};                                      // Screen start position of the Note On/Off message parameters 
    enum {CC_POS=0,};                                                       // Screen start position of the CC message parameters 
    enum {PROGRAM_CHANNEL_POS=0};                                           // Screen start position of the Program Change message parameters 
    enum {EDIT_GLOBAL_MODE_POS=5, EDIT_GLOBAL_KEY_POS=0, EDIT_GLOBAL_CHANNEL_POS=7}; // Screen start position of the Global Config parameters
    enum {STEP_NUM_POS=5, STEP_NOTE_POS=12, STEP_ENABLED_POS=9}; // Sequencer screen start position of the sequence step values
};
#endif
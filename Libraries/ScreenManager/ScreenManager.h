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

#define MSG_PAGE 0
#define MSG_TEMPO 1
#define MSG_BPM 2
#define MSG_EDIT 3
#define MSG_CHANNEL 4
#define MSG_NOTE_ON 5
#define MSG_NOTE_OFF 6
#define MSG_CTRL_CHANGE 7
#define MSG_CC 8
#define MSG_PGRM_CHANGE 9
#define MSG_PGM 10
#define MSG_VELOCITY 11

// Messages that will be displayed on the screen
const char msg_Page[] PROGMEM = "Page: ";
const char msg_Tempo[] PROGMEM = "Tempo: ";
const char msg_Bpm[] PROGMEM = " BPM";
const char msg_Edit[] PROGMEM = {'S','E','L','E','C','T',' ','A','\n','C','O','M','P','O','N','E','N','T','\0'};
const char msg_MsgChannel[] PROGMEM = "Ch:";
const char msg_NoteOn[] PROGMEM = "Note On";
const char msg_NoteOff[] PROGMEM = "Note Off";
const char msg_CtrlChange[] PROGMEM = "Ctrl Change";
const char msg_CC[] PROGMEM = "CC:";
const char msg_PgrmChange[] PROGMEM = "Pgrm Change";
const char msg_PGM[] PROGMEM = "PGM:";
const char msg_Velocity[] PROGMEM = "Vel:";

const char * const messages[] PROGMEM = {msg_Page, msg_Tempo, msg_Bpm, msg_Edit, msg_MsgChannel, msg_NoteOn, msg_NoteOff, msg_CtrlChange, msg_CC, msg_PgrmChange, msg_PGM, msg_Velocity};

class ScreenManager
{
  public:   
    void initialize(uint8_t i2cAddress, uint8_t rowLength, uint8_t rows);
    void printDefault(uint8_t page, uint16_t tempo);
    void printSelectComponent();
    void cleanScreen();
    uint8_t isComponentDisplayed();
    void displayPreviousMIDIMsg();
    void displayNextMIDIMsg();
    void displayComponentMIDIMessage(uint8_t msgIndex);
    void setMIDIComponentToDisplay(IMIDIComponent * midiComponent);
    IMIDIComponent * getDisplayedMIDIComponent();    

  private:
    void getMessage(uint8_t msgIndex, char * buffer);
    void printNoteOnOffMIDIMessage (MIDIMessage message);
    void printCCMIDIMessage (MIDIMessage message);
    void printPCMIDIMessage (MIDIMessage message);
    void printMIDIChannel(uint8_t channel);    
    
    IMIDIComponent * _displayedMIDIComponent;
    uint8_t _currentMIDIMessageDisplayed;
    LiquidCrystal_I2C _screen;     
};
#endif
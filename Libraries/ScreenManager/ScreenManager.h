/*
 * ScreenManager.h
 *
 * Class that manage the interactions between the MIDI controller and the sequencer with the screen
 *
 * Copyright 2018 3K MEDIALAB
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
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header
#include <avr/pgmspace.h>
#include <MIDIMessage.h>
#include <MIDI.h>
#include <IMIDIComponent.h>
#include <MIDIUtils.h>
#include <GlobalConfig.h>
#include <Step.h>
#include <ControllerConfig.h>
#include <stdlib.h>

#define MSG_PAGE 0
#define MSG_TEMPO 1
#define MSG_BPM 2
#define MSG_EDIT1 3
#define MSG_EDIT2 4
#define MSG_CHANNEL 5
#define MSG_NOTE_ON_OFF 6
#define MSG_CTRL_CHANGE 7
#define MSG_CC 8
#define MSG_PGRM_CHANGE 9
#define MSG_PGM 10
#define MSG_VELOCITY 11
#define MSG_SAVED 12
#define MSG_EMPTY_MIDI_TYPE 13
#define MSG_MODE 14
#define MSG_KEY 15
#define MSG_SEQ 16
#define MSG_STEP 17
#define MSG_STEP_LEGATO 18
#define MSG_STEP_ENABLED 19
#define MSG_PLAYBACK_MODE 20
#define MSG_STEP_SIZE 21
#define YES 22
#define NO 23
#define MSG_CLOCK 24
#define ON 25
#define OFF 26
#define MSG_PLAYBACK 27
#define MSG_CLK 28

// Messages that will be displayed on the screen that are stored into the PROGMEM
const char msg_Page[] PROGMEM = "Pg:";
const char msg_Tempo[] PROGMEM = "Tempo: ";
const char msg_Bpm[] PROGMEM = "BPM";
const char msg_Edit1[] PROGMEM = {"SELECT A"};
const char msg_Edit2[] PROGMEM = {"COMPONENT"};
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
const char msg_playback_mode[] PROGMEM = "Mode:";
const char msg_step_size[] PROGMEM = "Size:";
const char msg_Yes[] PROGMEM = "Yes";
const char msg_No[] PROGMEM = "No";
const char msg_Clock[] PROGMEM = "Clock:";
const char msg_On[] PROGMEM = "On";
const char msg_Off[] PROGMEM = "Off";
const char msg_Playback[] PROGMEM = "Playback:";
const char msg_Clk[] PROGMEM = "Clk:";

const char *const messages[] PROGMEM = {msg_Page, msg_Tempo, msg_Bpm, msg_Edit1, msg_Edit2, msg_MsgChannel, msg_NoteOnOff, msg_CtrlChange,
                                        msg_CC, msg_PgrmChange, msg_PGM, msg_Velocity, msg_saved, msg_empty_midi_type, msg_mode, msg_key, msg_seq, 
                                        msg_step, msg_step_legato, msg_step_enabled, msg_playback_mode, msg_step_size, msg_Yes, msg_No, msg_Clock, 
                                        msg_On, msg_Off, msg_Playback, msg_Clk};

class ScreenManager
{
public:
  void initialize();
  void printDefault(uint8_t page, uint8_t numPages, uint16_t tempo, uint8_t isMIDIClockOn);
  void printSelectComponentMessage();
  void printEditGlobalConfig(GlobalConfig globalConf);
  void printSavedMessage();
  void cleanScreen();
  uint8_t isComponentDisplayed();
  void displayPreviousMIDIMsg();
  void displayNextMIDIMsg();
  void displayComponentMIDIMessage(uint8_t msgIndex);
  void setMIDIComponentToDisplay(IMIDIComponent *midiComponent);
  IMIDIComponent *getDisplayedMIDIComponent();
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
  void printDefaultSequencer(uint8_t currentSequence, uint8_t totalSequences, uint16_t tempo, uint8_t playBackOn);
  void printEditSequencerConfig(char *playbackModeName, char *stepSizeName, uint8_t midiChannel, uint8_t sendClockWhilePlayback);
  void updateDisplayedPlaybackStep(Step step, uint8_t sequenceLength, uint8_t currentStep);
  void printEditStepData(Step step, uint8_t currentStep, uint8_t sequenceLength);
  void moveCursorToStepNote();
  void moveCursorToStepLegato();
  void moveCursorToStepEnabled();
  void moveCursorToPlayBackMode();
  void moveCursorToSendClockWhilePlayback();
  void moveCursorToStepSize();
  void moveCursorToSequencerMIDIChannel();
  void refreshStepNoteValue(uint8_t note);
  void refreshStepLegatoValue(uint8_t legato);
  void refreshStepEnabledValue(uint8_t enabled);
  void refreshDisplayedPlayBackMode(char *playBackMode);
  void refreshDisplayedSendClockWhilePlayback(uint8_t sendClockWhilePlayback);
  void refreshDisplayedStepSizeValue(char *stepSize);
  void refreshDisplayedSequencerMidiChannel(uint8_t midiChannel);

  uint8_t getDisplayedStepNumber();

private:
  void getMessage(uint8_t msgIndex, char *buffer);
  void append(char *s, char c);
  void printNoteOnOffMIDIData(MIDIMessage message);
  void printCCMIDIData(MIDIMessage message);
  void printPCMIDIData(MIDIMessage message);
  void clearRangeOnCurentLine(uint8_t row, uint8_t from, uint8_t to);
  char *getStepNoteValue(Step step);

  hd44780_I2Cexp _lcd;

  IMIDIComponent *_displayedMIDIComponent; // MIDI component currently assigned to the screen
  uint8_t _currentMIDIMessageDisplayed;    // MIDI message currently displayed on the screen
  uint8_t _currentDisplayedStep;           // Step currently displayed on the screen

  enum
  {
    MESSAGE_TYPE_POS = 4
  }; // Screen start position of the MIDI message type
  enum
  {
    NOTE_POS = 0,
    VELOCITY_POS = 5
  }; // Screen start position of the Note On/Off message parameters
  enum
  {
    CC_POS = 0,
  }; // Screen start position of the CC message parameters
  enum
  {
    PROGRAM_CHANNEL_POS = 0
  }; // Screen start position of the Program Change message parameters
  enum
  {
    EDIT_GLOBAL_MODE_POS = 5,
    EDIT_GLOBAL_KEY_POS = 4,
    EDIT_GLOBAL_CHANNEL_POS = 7
  }; // Screen start position of the Global Config parameters
  enum
  {
    STEP_NUM_POS = 5,
    STEP_NOTE_POS = 9,
    STEP_ENABLED_POS = 9,
    STEP_LEGATO_POS = 4
  }; // Sequencer screen start position of the sequence step values
  enum
  {
    SEQUENCER_EDIT_PLAYBACK_MODE_POS = 5,
    SEQUENCER_EDIT_SEND_CLOCK_POS = 9,
    SEQUENCER_EDIT_STEP_SIZE_POS = 5,
    SEQUENCER_EDIT_MIDI_CHANNEL_POS = 10
  }; // Screen start position of the Sequencer Config parameters
};
#endif
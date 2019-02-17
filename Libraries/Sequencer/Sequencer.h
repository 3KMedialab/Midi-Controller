/*
 * Sequencer.h
 *
 * Step sequencer implementation allowing three different playback modes (Forward, Backward and Random) and different step sizes (1/4, 1/8, 1/16 and 1/32) 
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

#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"
#include "MidiWorker.h"
#include "MIDIMessage.h"
#include "MIDI.h"
#include "Step.h"
#include "MemoryManager.h"
#include "ScreenManager.h"
#include "ControllerConfig.h"
#include "GlobalConfig.h"
#include "SyncManager.h"
#include <avr/pgmspace.h>

#define MICROSECONDS_PER_MINUTE 60000000

/* String characters inserted into PROGMEM area */
#define MSG_FORWARD 0
#define MSG_BACKWARD 1
#define MSG_RANDOM 2
#define MSG_QUARTER 3
#define MSG_EIGHTH 4
#define MSG_SIXTEENTH 5
#define MSG_THIRTYSECOND 6
#define MSG_NA 7

// Messages that will be displayed on the screen
const char msg_Forward[] PROGMEM = "Fwd";
const char msg_Backward[] PROGMEM = "Bwd";
const char msg_Random[] PROGMEM = "Rnd";
const char msg_Quarter[] PROGMEM = "1/4";
const char msg_Eighth[] PROGMEM = "1/8";
const char msg_Sixteenth[] PROGMEM = "1/16";
const char msg_ThirtySecond[] PROGMEM = "1/32";
const char msg_NA[] PROGMEM = "N/A";

const char *const sequencerMessages[] PROGMEM = {msg_Forward, msg_Backward, msg_Random, msg_Quarter, msg_Eighth, msg_Sixteenth, msg_ThirtySecond, msg_NA};

class Sequencer
{
public:
  Sequencer(uint8_t mode, uint8_t stepSize, MemoryManager *memoryManager, ScreenManager *screenManager);
  enum
  {
    FORWARD,
    BACKWARD,
    RANDOM
  }; // playback modes
  enum
  {
    QUARTER = 1,
    EIGHTH = 2,
    SIXTEENTH = 4,
    THIRTYSECOND = 8
  }; // step sizes
  enum
  {
    LENGTH = 16
  }; // number of steps within a sequence
  enum
  {
    PLAYBACK_MODE_TYPES = 3
  };

  uint8_t isPlayBackOn();
  uint8_t isStepSizeValueValid(uint8_t stepSizeValue);
  int8_t getPlayBackStep();
  uint8_t getPlayBackMode();
  uint8_t getPlayBackModeTypesNumber();
  uint8_t getMIDIChannel();
  uint8_t getSequenceLength();
  uint8_t getCurrentSequence();
  Step *getSequence();
  Step getSequenceStep(uint8_t pos);
  uint8_t getStepSize();

  void setMidiWorker(MidiWorker *midiWorker);
  void setPlayBackStep(int8_t currentStep);
  void setMIDIChannel(uint8_t channel);
  void setCurrentSequence(uint8_t numSequence);
  void setPlayBackMode(uint8_t mode);
  void setStepSize(uint8_t size);
  void setDisplayedStepNote(uint8_t note);
  void setDisplayedStepLegato(uint8_t legato);
  void setDisplayedStepEnabled(uint8_t enabled);
  void refreshDisplayedPlayBackMode(uint8_t playBackMode);
  void refreshDisplayedSendClockWhilePlayback(uint8_t sendClockWhilePlayback);
  void refreshDisplayedStepSizeValue(uint8_t stepSize);
  void refreshDisplayedMIDIChannel(uint8_t midiChannel);

  void loadCurrentSequence();
  void saveCurrentSequence();

  void startPlayBack();
  void stopPlayBack();
  uint8_t playBackSequence();
  void stopNote();

  void printDefault(SyncManager syncManager);
  void updateDisplayedStep();
  void printEditStepData();
  void printPreviousStep();
  void printNextStep();
  void printEditConfig(GlobalConfig globalConfig);
  void moveCursorToStepValue();
  void moveCursorToNote();
  void moveCursorToLegato();
  void moveCursorToEnabled();
  void moveCursorToPlayBackMode();
  void moveCursorToSendClockWhilePlayback();
  void moveCursorToStepSize();
  void moveCursorToMIDIChannel();
  void refreshDisplayedStepNote();

private:
  void playBackForward();
  void playBackBackward();
  void playBackRandom();
  void stopAllNotes();
  void stopCurrentNotePlayed();

  char *getPlayBackModeName();
  char *getPlayBackModeName(uint8_t playBackMode);
  char *getStepSizeName();
  char *getStepSizeName(uint8_t stepSize);

  void getMessage(uint8_t msgIndex, char *buffer);

  uint8_t _playBackOn;                      // 1 when playback is on, 0 otherwise
  int8_t _playBackStep;                     // current step being played
  uint8_t _midiChannel;                     // MIDI channel of the sequencer
  MidiWorker *_midiWorker;                  // Worker to deal with MIDI operations
  uint8_t _playBackMode;                    // playback mode (forward, Backward or Random)
  uint8_t _stepSize;                        // step size, where 1/4 is the length of a quarter note
  uint8_t _currentSequence;                 // current sequence assigned to the sequencer
  Step _steps[LENGTH];                      // steps array within a sequence
  uint8_t _stopCurrentPlayedNote;           // flag to stop current note being played
  uint8_t _currentNotePlayed;               // current note being played
  uint8_t _loadNewSequence;                 // indicates when a new sequence has been loaded

  MemoryManager *_memoryManager;            // Worker that manages memory load/store operations
  ScreenManager *_screenManager;            // Worker that manages screen display operations
};
#endif
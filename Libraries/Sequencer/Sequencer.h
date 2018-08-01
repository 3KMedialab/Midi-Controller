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

#define MSG_FORWARD 0
#define MSG_BACKWARD 1
#define MSG_RANDOM 2
#define MSG_QUARTER 3
#define MSG_EIGHTH 4
#define MSG_SIXTEENTH 5
#define MSG_THIRTYSECOND 6
#define MSG_NA 7

// Messages that will be displayed on the screen
const char msg_Forward[] PROGMEM = "Forward";
const char msg_Backward[] PROGMEM = "Backward";
const char msg_Random[] PROGMEM = "Random";
const char msg_Quarter[] PROGMEM = "1/4";
const char msg_Eighth[] PROGMEM = "1/8";
const char msg_Sixteenth[] PROGMEM = "1/16";
const char msg_ThirtySecond[] PROGMEM = "1/32";
const char msg_NA[] PROGMEM = "N/A";

const char * const sequencerMessages[] PROGMEM = {msg_Forward, msg_Backward, msg_Random, msg_Quarter, msg_Eighth, msg_Sixteenth, msg_ThirtySecond, msg_NA};

class Sequencer
{
  public:      

    Sequencer (uint8_t mode, uint8_t stepSize, MemoryManager * memoryManager, ScreenManager * screenManager);   
    enum {FORWARD, BACKWARD, RANDOM}; 
    enum {QUARTER=1, EIGHTH=2, SIXTEENTH=4, THIRTYSECOND=8};
    enum {LENGTH = 8};
    enum {PLAYBACK_MODE_TYPES = 3};

    uint8_t isPlayBackOn();
    uint8_t isStepSizeValueValid(uint8_t stepSizeValue);
    int8_t getPlayBackStep();
    uint8_t getPlayBackMode();
    uint8_t getPlayBackModeTypesNumber();
    uint8_t getMIDIChannel();
    uint8_t getSequenceLength();
    uint8_t getCurrentSequence();
    Step * getSequence();
    Step getSequenceStep(uint8_t pos);    
    uint8_t getStepSize();
     
    void setMidiWorker (MidiWorker * midiWorker);    
    void setPlayBackStep(int8_t currentStep);
    void setMIDIChannel(uint8_t channel);
    void setCurrentSequence(uint8_t numSequence);
    void setPlayBackMode(uint8_t mode);
    void setStepSize(uint8_t size);
    void setDisplayedStepNote(uint8_t note);
    void setDisplayedStepLegato(uint8_t legato);
    void setDisplayedStepEnabled(uint8_t enabled);
    void refreshDisplayedPlayBackMode(uint8_t playBackMode);
    void refreshDisplayedStepSizeValue(uint8_t stepSize);
    void refreshDisplayedMIDIChannel(uint8_t midiChannel);

    void loadCurrentSequence();
    void saveCurrentSequence();

    void startPlayBack();
    void stopPlayBack();
    uint8_t playBackSequence(SyncManager syncManager);
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
    void moveCursorToStepSize();
    void moveCursorToMIDIChannel();
    void refreshDisplayedStepNote();

  private:

    void playBackForward();
    void playBackBackward();
    void playBackRandom();    
    void stopAllNotes();
    void stopCurrentNotePlayed();

    char * getPlayBackModeName();
    char * getPlayBackModeName(uint8_t playBackMode);
    char * getStepSizeName();
    char * getStepSizeName(uint8_t stepSize);

    void getMessage(uint8_t msgIndex, char * buffer);

    uint8_t _playBackOn;   
    uint32_t _lastTimePlayBack;
    int8_t _playBackStep;
    uint8_t _midiChannel;
    MidiWorker * _midiWorker;
    uint8_t _playBackMode;    
    uint8_t _stepSize;
    uint8_t _currentSequence;
    Step _steps[LENGTH]; 
    uint8_t _stopCurrentPlayedNote;
    uint8_t _currentNotePlayed;
    uint8_t _loadNewSequence;
 
    MemoryManager * _memoryManager;
    ScreenManager * _screenManager;
};
#endif
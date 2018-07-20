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

#define MICROSECONDS_PER_MINUTE 60000000

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
    void setBpm(uint16_t * bpm);
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
    uint8_t playBackSequence(uint32_t currentTime);

    void printDefault();
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

    String getPlayBackModeName();
    String getPlayBackModeName(uint8_t playBackMode);
    String getStepSizeName();
    String getStepSizeName(uint8_t stepSize);

    uint8_t _playBackOn;
    uint16_t * _bpm;          
    uint32_t _lastTimePlayBack;
    int8_t _playBackStep;
    uint8_t _midiChannel;
    MidiWorker * _midiWorker;
    uint8_t _playBackMode;    
    uint8_t _stepSize;
    uint8_t _currentSequence;
    Step _steps[LENGTH]; 

    MemoryManager * _memoryManager;
    ScreenManager * _screenManager;
};
#endif
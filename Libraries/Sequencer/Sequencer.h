#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"
#include "MidiWorker.h"
#include "MIDIMessage.h"
#include "MIDI.h"
#include "Step.h"
#include <MemoryManager.h>
#include <ScreenManager.h>
#include <ControllerConfig.h>

#define MICROSECONDS_PER_MINUTE 60000000

class Sequencer
{
  public:      

    Sequencer (uint8_t mode, uint8_t stepSize, MemoryManager * memoryManager, ScreenManager * screenManager);   
    enum {FORWARD, BACKWARD, RANDOM}; 
    enum {QUARTER=1, EIGHTH=2, SIXTEENTH=4, THIRTYSECOND=8};
    enum {LENGTH = 8};

    uint8_t isPlayBackOn();
    int8_t getPlayBackStep();
    uint8_t getMIDIChannel();
    uint8_t getSequenceLength();
    uint8_t getCurrentSequence();
    Step * getSequence();
    Step getSequenceStep(uint8_t pos);
    
    void setMidiWorker (MidiWorker * midiWorker);
    void setBpm(uint16_t * bpm);
    void setPlayBackStep(int8_t currentStep);
    void setMIDIChannel(uint8_t channel);
    void setCurrentSequence(uint8_t numSequence);
    void setPlayBackMode(uint8_t mode);
    void setStepSize(uint8_t size);

    void loadCurrentSequence();

    void startPlayBack();
    void stopPlayBack();
    uint8_t playBackSequence(uint32_t currentTime);

    void printDefault();
    void updateDisplayedStep();

  private:

    void playBackForward();
    void playBackBackward();
    void playBackRandom();

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
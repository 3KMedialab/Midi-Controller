#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"
#include "MidiWorker.h"
#include "MIDIMessage.h"
#include "MIDI.h"
#include "Step.h"

#define MICROSECONDS_PER_MINUTE 60000000

class Sequencer
{
  public:      

    Sequencer (MidiWorker * midiWorker, uint8_t mode, uint8_t stepSize);   
    enum {FORWARD, BACKWARD, RANDOM}; 
    enum {QUARTER=1, EIGHTH=2, SIXTEENTH=4, THIRTYSECOND=8};
    enum {LENGTH = 8};

    uint8_t isPlayBackOn();
    int8_t getPlayBackStep();
    uint8_t getMIDIChannel();
    uint8_t getSequenceLength();
    Step * getSequence();
    Step getSequenceStep(uint8_t pos);
    
    void setBpm(uint8_t bpm);
    void setPlayBackStep(int8_t currentStep);
    void setMIDIChannel(uint8_t channel);
    void setPlayBackMode(uint8_t mode);
    void setStepSize(uint8_t size);

    void startPlayBack();
    void stopPlayBack();
    void playBackSequence(uint32_t currentTime);

  private:

    void playBackForward();
    void playBackBackward();
    void playBackRandom();

    uint8_t _playBackOn;
    uint32_t _bpm;          
    uint32_t _lastTimePlayBack;
    int8_t _playBackStep;
    uint8_t _midiChannel;
    MidiWorker * _midiWorker;
    uint8_t _playBackMode;    
    uint8_t _stepSize;
    Step _steps[LENGTH]; 
};
#endif
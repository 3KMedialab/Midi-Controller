#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"
#include "Sequence.h"
#include "MidiWorker.h"
#include "MIDIMessage.h"
#include "MIDI.h"

class Sequencer
{
  public:      

    Sequencer (MidiWorker * midiWorker, Sequence * sequence);    

    Sequence * getSequence();
    uint32_t getStepDelay();
    uint32_t getLastTimePlayBack();
    uint8_t isPlayBackOn();
    uint8_t getPlayBackStep();
    uint8_t getMIDIChannel();
    
    void setSequence(Sequence * sequence);
    void startPlayBack();
    void stopPlayBack();
    void setStepDelay(uint32_t delay);
    void setLastTimePlayBack(uint32_t time);
    void setPlayBackStep(uint8_t currentStep);
    void setMIDIChannel(uint8_t channel);

    void playBackSequence(uint32_t currentTime);

  private:

    Sequence * _sequence;
    uint8_t _playBackOn;
    uint32_t _stepDelay;          // holds the delay between steps playback
    uint32_t _lastTimePlayBack;
    uint8_t _playBackStep;
    uint8_t _midiChannel;
    MidiWorker * _midiWorker;
    MIDIMessage _message;


  
};
#endif
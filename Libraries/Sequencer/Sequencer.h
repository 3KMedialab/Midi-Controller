#ifndef Sequencer_h
#define Sequencer_h

#include "Arduino.h"
#include "Sequence.h"
#include "MidiWorker.h"
#include "MIDIMessage.h"
#include "MIDI.h"

#define MICROSECONDS_PER_MINUTE 60000000

class Sequencer
{
  public:      

    Sequencer (MidiWorker * midiWorker, Sequence * sequence);    

    Sequence * getSequence();
    uint8_t isPlayBackOn();
    uint8_t getPlayBackStep();
    uint8_t getMIDIChannel();
    
    void setSequence(Sequence * sequence);
    void startPlayBack();
    void stopPlayBack();
    void setBpm(uint8_t bpm);
    void setPlayBackStep(uint8_t currentStep);
    void setMIDIChannel(uint8_t channel);

    void playBackSequence(uint32_t currentTime);

  private:

    Sequence * _sequence;
    uint8_t _playBackOn;
    uint32_t _bpm;          
    uint32_t _lastTimePlayBack;
    uint8_t _playBackStep;
    uint8_t _midiChannel;
    MidiWorker * _midiWorker;
    MIDIMessage _message;


  
};
#endif
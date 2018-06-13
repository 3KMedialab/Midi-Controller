#ifndef MidiWorker_h
#define MidiWorker_h

#include <MIDI.h>
#include <MIDIMessage.h>

typedef midi::MidiInterface<HardwareSerial> MidiInterface;

class MidiWorker
{
  public:   
    MidiWorker();   
    MidiWorker(MidiInterface& inInterface);   
    void begin();
    void sendMIDIMessage(MIDIMessage * message, uint8_t channel);
    void sendMIDIStartClock();
    void sendMIDIClock();
    void sendMIDIStopClock();

  private:

    MidiInterface& _mMidi;
   
};
#endif
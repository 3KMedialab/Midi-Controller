/* 
    MIDIController.h - 
    Created by: Daniel Segura, August 30, 2017
*/

#ifndef MIDIController_h
#define MIDIController_h

#include <MIDI.h>
#include <MIDIButton.h>
#include <MIDIPotentiometer.h> 

typedef midi::MidiInterface<HardwareSerial> MidiInterface;

class MIDIController
{
  public:   
    MIDIController(MidiInterface& inInterface, Button * shiftButton, MIDIButton * midiButtons, MIDIPotentiometer * midiPots, uint8_t numMIDIButtons, uint8_t numMIDIPots);
      
    void processMIDIButtons();
    void processMIDIPots();
    void begin();
    uint8_t wasShiftButtonReleased();
    uint8_t isShiftMode();
    void setMIDIButtons(MIDIButton * midiButtons);
    void setMIDIPotentiometers(MIDIPotentiometer * midiPots);    
    void setShiftMode(uint8_t shiftMode);

  private:
    uint8_t _numMIDIButtons;
    uint8_t _numMIDIPots;
    MIDIButton * _midiButtons;
    MIDIPotentiometer * _midiPots;
    Button *_shiftButton;
    uint8_t _shiftMode = 0;
    MidiInterface& _mMidi;

    void sendMIDIMessage(MIDIMessage message);

};
#endif
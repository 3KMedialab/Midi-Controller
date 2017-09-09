/* 
    MIDIController.h - 
    Created by: Daniel Segura, August 30, 2017
*/

#ifndef MIDIController_h
#define MIDIController_h

#include <MIDI.h>
#include <MIDIButton.h>
#include <MIDIPotentiometer.h> 
#include <ControllerConfig.h>
#include <Led.h>

typedef midi::MidiInterface<HardwareSerial> MidiInterface;

class MIDIController
{
  public:   
    MIDIController(MidiInterface& inInterface, MIDIButton * midiButtons, MIDIPotentiometer * midiPots, uint8_t numMIDIButtons, uint8_t numMIDIPots);
    MIDIController(MIDIButton * midiButtons, MIDIPotentiometer * midiPots, uint8_t numMIDIButtons, uint8_t numMIDIPots);
      
    void processMIDIButtons();
    void processMIDIPots();
    void begin();  
    void processShiftButton(MIDIButton * midiButtons, MIDIPotentiometer * midiPots);
    uint8_t wasShiftButtonReleased();
    uint8_t isShiftMode();

  private:
    uint8_t _numMIDIButtons;
    uint8_t _numMIDIPots;
    MIDIButton * _midiButtons;
    MIDIPotentiometer * _midiPots;
    Button _shiftButton = Button(SHIFT_BUTTON_PIN, PULLUP, INVERT, DEBOUNCE_MS);
    Led _shiftButtonLed = Led(SHIFT_BUTTON_LED_PIN);
    uint8_t _shiftMode = 0;
    MidiInterface& _mMidi;

    void sendMIDIMessage(MIDIMessage message);
    void printSerial(MIDIMessage message);

};
#endif
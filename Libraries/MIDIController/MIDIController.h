/* 
    MIDIController.h - 
    Created by: Daniel Segura, August 30, 2017
*/

#ifndef MIDIController_h
#define MIDIController_h

#include <MIDI.h>
#include <MIDIButton.h>
#include <MIDIPotentiometer.h> 
#include <Pitches.h>

class MIDIController
{
  public:   
    MIDIController(MIDIButton * noteOnOffButtons, MIDIPotentiometer * controlChangePots);
    
   /* uint8_t getNumNoteOnOffButtons();
    uint8_t getNumControlChangePots();
    
    MIDIButton * getNoteOnOffButtons();
    MIDIPotentiometer * getControlChangePots();

    void initNoteOnOffButtons();
    void initControlChangePots();
    void processNoteOnOffButtons();
    void processControlChangePots();*/

  private:
    uint8_t _numNoteOnOffButtons;
    uint8_t _numControlChangePots;

    MIDIButton * _noteOnOffButtons;
    MIDIPotentiometer * _controlChangePots;

};
#endif

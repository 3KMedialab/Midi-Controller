/* 
    MIDIController.cpp - 
    Created by: Daniel Segura, August 30, 2017
*/

#include "MIDIController.h"

/**
  Constructor for MIDI Controller.

  @pin 
*/

MIDIController::MIDIController(MIDIButton * noteOnOffButtons, MIDIPotentiometer * controlChangePots)
{  
    _noteOnOffButtons = noteOnOffButtons;
    _controlChangePots = controlChangePots;
    
    _numNoteOnOffButtons = sizeof(_noteOnOffButtons)/sizeof(MIDIButton);
    _numControlChangePots = sizeof(_controlChangePots)/sizeof(MIDIPotentiometer);  
}

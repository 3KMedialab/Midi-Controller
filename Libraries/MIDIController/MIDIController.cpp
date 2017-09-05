/* 
    MIDIController.cpp - 
    Created by: Daniel Segura, August 30, 2017
*/

#include "MIDIController.h"

/**
  Constructor for MIDI Controller.

  @pin 
*/

MIDIController::MIDIController(MidiInterface& inInterface, Button * shiftButton, MIDIButton * midiButtons, MIDIPotentiometer * midiPots, uint8_t numMIDIButtons, uint8_t numMIDIPots) 
    : _mMidi(inInterface)
{
    _midiButtons = midiButtons;
    _midiPots = midiPots;
    
    _numMIDIButtons = numMIDIButtons;
    _numMIDIPots = numMIDIPots;

    _shiftButton = shiftButton;
}

void MIDIController::begin()
{
   _mMidi.begin();
}

uint8_t MIDIController::wasShiftButtonReleased()
{
    (*_shiftButton).read();
    return (*_shiftButton).wasReleased();
}

uint8_t MIDIController::isShiftMode()
{
    return _shiftMode;
}

void MIDIController::setMIDIButtons(MIDIButton * midiButtons)
{
    _midiButtons = midiButtons;
}

void MIDIController::setMIDIPotentiometers (MIDIPotentiometer * midiPots)
{   
    _midiPots = midiPots;
}

void MIDIController::setShiftMode(uint8_t shiftMode)
{
    _shiftMode = shiftMode;
}

void MIDIController::processMIDIPots()
{   
    for (int i = 0; i < _numMIDIPots; i++)
    {
       if (_midiPots[i].wasChanged())
       {
           // send MIDI message          
           sendMIDIMessage(_midiPots[i].getMessage());           
       }
    }
}

void MIDIController::processMIDIButtons()
{  
    for (int i = 0; i < _numMIDIButtons; i++)
    {
        _midiButtons[i].read();

        if (_midiButtons[i].wasPressed())
        {
            // send MIDI message          
           sendMIDIMessage(_midiButtons[i].getOnPressedMessage());
        }

        if (_midiButtons[i].wasReleased())
        {
            // send MIDI message            
            sendMIDIMessage(_midiButtons[i].getOnReleasedMessage());
        }       
    }
}

/*
* Send a MIDI message regarding its type
*/
void MIDIController::sendMIDIMessage(MIDIMessage message)
{ 
    switch(message.getType())
    {
        case midi::ControlChange:
           _mMidi.sendControlChange(message.getDataByte1(), message.getDataByte2(), message.getChannel());
        break;

        case midi::ProgramChange:
           _mMidi.sendProgramChange(message.getDataByte1(), message.getChannel());
        break;

        case midi::NoteOn:
           _mMidi.sendNoteOn(message.getDataByte1(), message.getDataByte2(), message.getChannel());
        break;

        case midi::NoteOff:
           _mMidi.sendNoteOff(message.getDataByte1(), message.getDataByte2(), message.getChannel());
        break;
    }
} 
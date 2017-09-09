/* 
    MIDIController.cpp - 
    Created by: Daniel Segura, August 30, 2017
*/

#include "MIDIController.h"

/**
  Constructor for MIDI Controller.

  @pin 
*/

MIDIController::MIDIController(MidiInterface& inInterface, MIDIButton * midiButtons, MIDIPotentiometer * midiPots, uint8_t numMIDIButtons, uint8_t numMIDIPots) 
    : _mMidi(inInterface)
{
    _midiButtons = midiButtons;
    _midiPots = midiPots;
    
    _numMIDIButtons = numMIDIButtons;
    _numMIDIPots = numMIDIPots;  
}

MIDIController::MIDIController(MIDIButton * midiButtons, MIDIPotentiometer * midiPots, uint8_t numMIDIButtons, uint8_t numMIDIPots) 
{
    _midiButtons = midiButtons;
    _midiPots = midiPots;

    _numMIDIButtons = numMIDIButtons;
    _numMIDIPots = numMIDIPots;
}

void MIDIController::begin()
{
   _mMidi.begin();
}

uint8_t MIDIController::wasShiftButtonReleased()
{
    _shiftButton.read();
    return _shiftButton.wasReleased();
}

void MIDIController::processShiftButton(MIDIButton * midiButtons, MIDIPotentiometer * midiPots)
{
    _midiButtons = midiButtons;
    _midiPots = midiPots;   
    _shiftMode = !_shiftMode; 
    _shiftButtonLed.setState(_shiftMode);
}

uint8_t MIDIController::isShiftMode()
{
    return _shiftMode;
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

void MIDIController::printSerial(MIDIMessage message)
{ 
    switch(message.getType())
    {
        case midi::ControlChange:
          Serial.println("ControlChange");
          Serial.println(message.getDataByte1(),DEC);
          Serial.println(message.getDataByte2(),DEC);
          Serial.println(message.getChannel(),DEC);
        break;

        case midi::ProgramChange:
        Serial.println("ProgramChange");
        Serial.println(message.getDataByte1(),DEC);
        Serial.println(message.getDataByte2(),DEC);
        Serial.println(message.getChannel(),DEC);         
        break;

        case midi::NoteOn:
        Serial.println("NoteOn");
        Serial.println(message.getDataByte1(),DEC);
        Serial.println(message.getDataByte2(),DEC);
        Serial.println(message.getChannel(),DEC);               
        break;

        case midi::NoteOff:
        Serial.println("NoteOff");
        Serial.println(message.getDataByte1(),DEC);
        Serial.println(message.getDataByte2(),DEC);
        Serial.println(message.getChannel(),DEC);            
        break;
    }
} 
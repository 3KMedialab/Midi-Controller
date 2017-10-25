/*
 * MIDIController.cpp
 *
 * The MIDI Controller class. It contains an array of MIDIButtons and MIDIPotentiometers. 
 * It also contains a button that implemts the shift function and two leds to indicate 
 * the current shift mode. 
 *
 * The configuration of the controller is stored in the ControllerConfig.h file
 *
 * Copyright 2017 3K MEDIALAB
 *   
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "MIDIController.h"

/*
* Constructor
* inInterface: the MIDI interface that the controller will use.
* midiButtons: the array of MIDI components the controller will manage.
* numMIDIComponents: number of MIDI components assigned to the controllerto use.
*/
MIDIController::MIDIController(MidiInterface& inInterface, IMIDIComponent ** midiComponents, uint8_t numMIDIComponents) 
: _mMidi(inInterface)
{
    _midiComponents = midiComponents;
    _numMIDIComponents = numMIDIComponents;
}

/*
* Constructor (for debug purposes)
* midiButtons: the array of MIDIButtons the controller will manage.
* midiPots: tha array of potentiometers the controller will manage.
* numMIDIButtons: number of MIDI buttons to use.
* numMIDIPots: number of MIDI Potentiometers to use.
*/
MIDIController::MIDIController(IMIDIComponent ** midiComponents, uint8_t numMIDIComponents) 
{
    _midiComponents = midiComponents;
    _numMIDIComponents = numMIDIComponents;
}

/*
* Initializes the MIDI interface
*/
void MIDIController::begin()
{
   _mMidi.begin();
}

/*
* Checks if the shift button was released.
* Return true if the shift button was pressed and released.
*/
/*uint8_t MIDIController::wasShiftButtonReleased()
{
    _shiftButton.read();
    return _shiftButton.wasReleased();
}*/

/*
* Process the shift function: Changes the assigned MIDI buttons and MIDI Potentiometers of the MIDI Controller and
* set the new shift and leds state.
* midiButtons: the new array of buttons that the MIDI Controller will manage.
* midiPots: the new array of potentiometers that the MIDI Controller will manage.
*/
/*void MIDIController::processShiftButton(MIDIButton * midiButtons, MIDIPotentiometer * midiPots)
{
    _midiButtons = midiButtons;
    _midiPots = midiPots;   
    _shiftMode = !_shiftMode; 
    _shiftButtonLed1.setState(!_shiftButtonLed1.getState());
    _shiftButtonLed2.setState(!_shiftButtonLed2.getState());
}*/

/*
* Returns the shift state of the MIDI Controller.
* 0 if the Controller is not in shift mode, 1 if the controller is in shift mode.
*/
/*uint8_t MIDIController::isShiftMode()
{
    return _shiftMode;
}*/

/* 
* Process the MIDI components of the MIDI Controller, and sends the MIDI Message assingned to each component event.
*/
void MIDIController::processMIDIComponents()
{
    for (int i = 0; i < _numMIDIComponents; i++)
    {
        processMidiComponent(_midiComponents[i]);
    }
}

/* 
* Send the MIDI message of the MIDI component
*/
void MIDIController::processMidiComponent(IMIDIComponent * component)
{
    MIDIMessage * message = component->getMessageToSend();
    
    if (message != NULL)
    {
        sendMIDIMessage(message); 
    }
}

/*
* Send a MIDI message regarding its type
* message: the MIDI message.
*/
void MIDIController::sendMIDIMessage(MIDIMessage * message)
{ 
    switch(message->getType())
    {
        case midi::ControlChange:
           _mMidi.sendControlChange(message->getDataByte1(), message->getDataByte2(), message->getChannel());
        break;

        case midi::ProgramChange:
           _mMidi.sendProgramChange(message->getDataByte1(), message->getChannel());
        break;

        case midi::NoteOn:
           _mMidi.sendNoteOn(message->getDataByte1(), message->getDataByte2(), message->getChannel());
        break;

        case midi::NoteOff:
           _mMidi.sendNoteOff(message->getDataByte1(), message->getDataByte2(), message->getChannel());
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
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
* midiComponents: the array of MIDI components the controller will manage.
* numMIDIComponents: number of MIDI components assigned to the controller
*/
MIDIController::MIDIController(MidiInterface& inInterface, IMIDIComponent ** midiComponents, uint8_t numMIDIComponents) 
: _mMidi(inInterface)
{
    _midiComponents = midiComponents;
    _numMIDIComponents = numMIDIComponents;     
}

/*
* Constructor (for debug purposes)
* midiComponents: the array of MIDI components the controller will manage.
* numMIDIComponents: number of MIDI components assigned to the controller
*/
MIDIController::MIDIController(IMIDIComponent ** midiComponents, uint8_t numMIDIComponents) 
{
    _midiComponents = midiComponents;
    _numMIDIComponents = numMIDIComponents;
    _currentPage = 1;
}

/*
* Initializes the MIDI controller
*/
void MIDIController::begin()
{
   // Initializes the MIDI interface
   _mMidi.begin();

   _memoryManager.initialize(_midiComponents, _numMIDIComponents);
   _screenManager.initialize(I2C_ADDRESS, ROW_LENGTH, ROWS);

   // load from EEPROM the default page of MIDI messages into the MIDI components
   _currentPage = 1;   
   _memoryManager.loadMIDIComponents(_currentPage, _midiComponents, _numMIDIComponents);

   // set the default tempo
   _bpm = map(_bpmPot.getSmoothValue(), 0, 1023, MIN_BPM, MAX_BPM);

   // don't send MIDI clock by default
   _midiClock = 0;

   // variable that controls the bpm led blink frequency
   _lastTime = 0;

   _lastTimeClock = 0;

   _screenManager.cleanScreen();
   _screenManager.printDefault(_currentPage, _bpm);
}

/*
* Process the buttons for load pages of MIDI messages into the MIDI components
*/
void MIDIController::processPageButtons()
{
    // process page decrease button
    _decPageButton.read();
    if (_decPageButton.wasPressed() && _currentPage > 1)
    {
        _currentPage -= 1;
        loadPage(_currentPage);
        _screenManager.printDefault(_currentPage, _bpm);
    }

    // process page increase button
    _incPageButton.read();
    if (_incPageButton.wasPressed() && _currentPage < _memoryManager.getMaxPages())
    {
        _currentPage += 1;
        loadPage(_currentPage);
        _screenManager.printDefault(_currentPage, _bpm);
    }
}

/* 
* Process the MIDI components of the MIDI Controller
*/
void MIDIController::processMIDIComponents()
{
    for (int i = 0; i < _numMIDIComponents; i++)
    {
        processMidiComponent(_midiComponents[i]);
    }
}

/* 
* Process a MIDI component in order to send the corresponding MIDI Message regarding the component event triggered
* component: the MIDI component to process.
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
* message: the MIDI message to be sent.
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

/*
* Send MIDI clock signal
*/
void MIDIController::sendMIDIRealTime(uint8_t inType)
{
    switch(inType)
    {
        case midi::Start:
            _mMidi.sendRealTime(midi::Start);
        break;

        case midi::Clock:
            _mMidi.sendRealTime(midi::Clock);
        break;
        
        case midi::Stop:
            _mMidi.sendRealTime(midi::Stop);
        break;
        
        default:
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

/*
* Save the MIDI messages currently assigned to each MIDI component in the EEPROM
* page: page number where the MIDI messages will be saved.
*/
void MIDIController::savePage(uint8_t page)
{        
    _memoryManager.saveMIDIComponents(page, _midiComponents, _numMIDIComponents);
}

/*
* Load the MIDI messages stored in a page in the EEPROM into the MIDI components
* page: page number which contains the MIDI messages that will be loaded into the MIDI components
*/
void MIDIController::loadPage(uint8_t page)
{        
    _memoryManager.loadMIDIComponents(page, _midiComponents, _numMIDIComponents);
}

/*
* Check if the tempo potentiometer has changed and calculates the new value of the controller's tempo in BPM
*/
void MIDIController::processTempoPot()
{        
    if (_bpmPot.wasChanged())
    {
        _bpm = map(_bpmPot.getSmoothValue(), 0, 1023, MIN_BPM, MAX_BPM);
        _screenManager.printDefault(_currentPage, _bpm);
        
        // calculate the blink frequency of the LED
        uint32_t millisecondsPerMinute = 60000;
        _delayMS = millisecondsPerMinute / _bpm;       
    }
}

/*
* Process the button that activates/deactivates sending of MIDI clock signal.
* In case of the controller is sending MIDI clock, then the led blinks at BPM frequency
*/
void MIDIController::processMIDIClockComponents()
{
    // activate/deactivate MIDI clock signal sending
    _midiClockButton.read();

    if (_midiClockButton.wasPressed())
    {
        _midiClock = !_midiClock;

        if (_midiClock == 1)
        {
            sendMIDIRealTime(midi::Start);   
        }
        else
        {
            sendMIDIRealTime(midi::Stop);
            _bpmLed.setState(LOW);  
        }
    }

    uint32_t currentTime = millis();
   
    // send MIDI clock signal regarding the tempo
    if (currentTime - _lastTimeClock >= (_delayMS / 24))
    {                       
        if (_midiClock == 1)
        {    
            sendMIDIRealTime(midi::Clock);
            _lastTimeClock = currentTime;
        }    
    }

    // controls led blinking regarding the tempo
    if (currentTime - _lastTime >= (_delayMS / 2))
    {
        if (_midiClock == 1)
        {          
            _bpmLed.setState(!_bpmLed.getState());
            _lastTime =  currentTime;
        }        
    }
}
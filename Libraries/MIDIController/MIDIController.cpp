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
   _wasPageSaved = 0;

   // set the default tempo
   _bpm = map(_selectValuePot.getSmoothValue(), 0, 1023, MIN_BPM, MAX_BPM);  

   // variable that controls the bpm led blink frequency
   _lastTime = 0;

   _lastTimeClock = 0;

   _screenManager.cleanScreen();
   _screenManager.printDefault(_currentPage, _bpm);

   // Controller initial state and substate
   _state = CONTROLLER;
   _subState = MIDI_CLOCK_OFF; 
}

/*
* Process the buttons for load pages of MIDI messages into the MIDI components
*/
void MIDIController::processIncDecButtons()
{
    /**************************** DECREASE BUTTON **********************/
    _decPageButton.read();

    if (_decPageButton.wasPressed())
    {
        switch (_state)
        {        
            // load the previous page from memory
            case CONTROLLER:
            
                if (_currentPage > 1)
                {
                    _currentPage -= 1;
                    loadPage(_currentPage);
                    _screenManager.printDefault(_currentPage, _bpm);
                }

            break;

            // display the previous MIDI message of the component loaded into the Screen Manager
            case EDIT:
                
                if (_subState != DEFAULT_EDIT_MSG)
                {
                    _screenManager.displayPreviousMIDIMsg();
                    _subState = EDIT_MIDI_TYPE;
                }

            break;       
        }
    }
    /**************************** END DECREASE BUTTON **********************/

    /**************************** INCREASE BUTTON **********************/
    _incPageButton.read();

    if (_incPageButton.wasPressed())
    {
        switch (_state)
        {
            // load the next page from memory
            case CONTROLLER:
                
                if (_currentPage < _memoryManager.getMaxPages())
                {
                    _currentPage += 1;
                    loadPage(_currentPage);
                    _screenManager.printDefault(_currentPage, _bpm);
                }   
            
            break;
           
            // display the next MIDI message of the component loaded into the Screen Manager
            case EDIT:
            
                if (_subState != DEFAULT_EDIT_MSG)
                {
                    _screenManager.displayNextMIDIMsg();
                    _subState = EDIT_MIDI_TYPE;
                }          
            
            break;
        }
    }   
    /**************************** INCREASE BUTTON **********************/
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
    // edit mode is off: send MIDI message
    switch (_state)
    {
        // send a MIDI message
        case CONTROLLER:
        {            
            MIDIMessage * message = component->getMessageToSend();
            
            if (message != NULL)
            {
                sendMIDIMessage(message); 
            }
        
            break;
        }
        
        // assign the MIDI messages information to the screen and display the first MIDI message
        case EDIT:
        {
            if (component->wasActivated())
            {
                if (_screenManager.getDisplayedMIDIComponent() != component)
                {
                    _screenManager.setMIDIComponentToDisplay(component);
                    _screenManager.displayComponentMIDIMessage(1); 
                    _subState = EDIT_MIDI_TYPE;
                }            
            }

            break;
        }
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
* Process the potentiometer that set the value for controller's 
* tempo or set the values for MIDI messages on edit mode
*/
void MIDIController::processSelectValuePot()
{        
    if (_selectValuePot.wasChanged())
    {
        switch (_state)
        {
            // set controller's tempo and calculate delays for led blinking and MIDI clock signal sending
            case CONTROLLER:
            {
                _bpm = map(_selectValuePot.getSmoothValue(), 0, 1023, MIN_BPM, MAX_BPM);
                _screenManager.printDefault(_currentPage, _bpm);
                
                // calculate the blink frequency of the LED
                uint32_t microsecondsPerMinute = 60000000;
                _delayLedMS = (microsecondsPerMinute / _bpm) / 2;    
                _delayClockTickMS = (microsecondsPerMinute / _bpm) / 24;   
            
                break;
            } 

            // select a value for a midi message part
            case EDIT:
            {
                // get the component displayed on screen currently
                IMIDIComponent * displayedComponent = _screenManager.getDisplayedMIDIComponent();             
                uint8_t displayedMessageIndex = (_screenManager.getDisplayedMessageIndex())-1;

                switch (_subState)
                {
                    case EDIT_MIDI_TYPE:    
                    {
                        // select the new MIDI message type
                        _selectValuePot.setSectors(displayedComponent->getNumAvailableMessageTypes());

                        // set the new MIDI message type into the component and refresh the screen
                        uint8_t * availableMIDIMessages = displayedComponent->getAvailableMessageTypes();
                        displayedComponent->getMessages()[displayedMessageIndex].setType(availableMIDIMessages[_selectValuePot.getSector()]);
                        if (_selectValuePot.isNewSector())
                        {
                            _screenManager.refreshMIDIData();
                        }
                        break;
                    }

                    case EDIT_NOTE:
                    {
                        // set the new note value in to the component
                        uint8_t note = map(_selectValuePot.getSmoothValue(), 0, 1023, NOTE_C_1, NOTE_G9);
                        displayedComponent->getMessages()[displayedMessageIndex].setDataByte1(note);

                        // print the new note value on the screen
                        _screenManager.refreshNoteValue(note);                      
                   
                        break;
                    }

                    case EDIT_VELOCITY:
                    {

                        //set the new velocity value into the component
                        uint8_t velocity = map(_selectValuePot.getSmoothValue(), 0, 1023, 0, 127);
                        displayedComponent->getMessages()[displayedMessageIndex].setDataByte2(velocity);

                        // print the new velocity value on the screen
                        _screenManager.refreshVelocityValue(velocity);                          

                        break;
                    }

                    case EDIT_CC:
                    {
                        //set the new control change type value into the component
                        uint8_t ccValue = map(_selectValuePot.getSmoothValue(), 0, 1023, 0, 127);
                        displayedComponent->getMessages()[displayedMessageIndex].setDataByte1(ccValue);

                        // print the new cc value on the screen
                        _screenManager.refreshCCValue(ccValue);   
                        
                        break;
                    }

                    case EDIT_CHANNEL:
                    {
                        //set the new channel value into the component
                        uint8_t channelValue = map(_selectValuePot.getSmoothValue(), 0, 1023, 1, 16);
                        displayedComponent->getMessages()[displayedMessageIndex].setChannel(channelValue);
                        
                        // print the new channel value on the screen
                        _screenManager.refreshChannelValue(channelValue);   
                        
                        break;
                    }
                    break;                   
                }

                break;
            }
        }  
    }
}

void MIDIController::processMultiplePurposeButton()
{
    _multiplePurposeButton.read();
    
    if (_multiplePurposeButton.wasPressed())
    {
        switch (_state)
        {
            // activate/deactivate MIDI clock signal sending
            case CONTROLLER:                
                updateMIDIClockState();
            break;

           case EDIT:
                moveCursorToValue();                          
           break;
        }    
    }  
}


void MIDIController::updateMIDIClockState()
{               
    _subState == MIDI_CLOCK_OFF ? _subState=MIDI_CLOCK_ON : _subState=MIDI_CLOCK_OFF;  
            
    switch (_subState)
    {
        // send start real time message
        case MIDI_CLOCK_ON:
            sendMIDIRealTime(midi::Start);  
        break;

        // send stop realtime message
        case MIDI_CLOCK_OFF:
            sendMIDIRealTime(midi::Stop);
            _bpmLed.setState(LOW);  
        break;
    }           
}

void MIDIController::moveCursorToValue()
{
    // get the current MIDI message type displayed on the screen, set the next subState and move the cursor to the next value to edit 
    switch (_screenManager.getDisplayedMessageType())
    {                    
        case midi::ControlChange:

            switch (_subState)
            {
                case EDIT_MIDI_TYPE:

                    _subState = EDIT_CC;
                    _screenManager.moveCursorToCC();

                break;

                case EDIT_CC:

                    _subState = EDIT_CHANNEL;
                    _screenManager.moveCursorToChannel();

                break;

                case EDIT_CHANNEL:
                
                    _subState = EDIT_MIDI_TYPE;
                    _screenManager.moveCursorToMsgType();

                break;
            }
        
        break;

        case midi::ProgramChange:

            switch (_subState)
            {
                case EDIT_MIDI_TYPE:

                    _subState = EDIT_CHANNEL;
                    _screenManager.moveCursorToChannel();

                break;                          

                case EDIT_CHANNEL:
                
                    _subState = EDIT_MIDI_TYPE;
                    _screenManager.moveCursorToMsgType();

                break;
            }
            
        break;

        case midi::NoteOn:
        case midi::NoteOff:

            switch (_subState)
            {
                case EDIT_MIDI_TYPE:

                    _subState = EDIT_NOTE;
                    _screenManager.moveCursorToNote();

                break;

                case EDIT_NOTE:

                    _subState = EDIT_VELOCITY;
                    _screenManager.moveCursorToVelocity();

                break;

                case EDIT_VELOCITY:
                
                    _subState = EDIT_CHANNEL;
                    _screenManager.moveCursorToChannel();
                
                break;
                
                case EDIT_CHANNEL:
                
                    _subState = EDIT_MIDI_TYPE;
                    _screenManager.moveCursorToMsgType();

                break;
            }            
        break;
    }
}    

void MIDIController::sendMIDIClock()
{
    uint32_t currentTime = micros();
    
     // send MIDI clock signal regarding the tempo
     if (currentTime - _lastTimeClock >= _delayClockTickMS)
     {                       
         if (_state == CONTROLLER && _subState == MIDI_CLOCK_ON)
         {    
             sendMIDIRealTime(midi::Clock);
             _lastTimeClock = currentTime;
         }    
     }
 
     // controls led blinking regarding the tempo
     if (currentTime - _lastTime >= _delayLedMS)
     {
         if (_state == CONTROLLER && _subState == MIDI_CLOCK_ON)
         {          
             _bpmLed.setState(!_bpmLed.getState());
             _lastTime =  currentTime;
         }        
     }
}

/*
* Process the button that activates/deactivates the edit mode.
*/
void MIDIController::processEditModeButton()
{
    // activate/deactivate edit mode
    _editButton.read();

    if (_editButton.wasReleased())
    {
        if (!_wasPageSaved)
        {
            switch (_state)
            {
                // stop sending MIDI Clock, LED blinks permanently and defaut edit message is displayed on screen
                case CONTROLLER:

                    // stop sending MIDI Clock signal in case of the controller was sending such signal
                    if (_subState == MIDI_CLOCK_ON)
                        sendMIDIRealTime(midi::Stop);        
    
                    _state = EDIT;
                    _subState = DEFAULT_EDIT_MSG;                
                        
                    _bpmLed.setState(HIGH); 
                    _screenManager.printSelectComponentMessage();
                
                break;
    
                // LED doesn't blink, default message is displayed on screen 
                case EDIT:
    
                    _state = CONTROLLER;
                    _subState = MIDI_CLOCK_OFF;    
                
                    _bpmLed.setState(LOW);          
                    _screenManager.printDefault(_currentPage, _bpm);      
                    
                break;
            }
        }       
   
        // after saving a page, set controller status to CONTROLLER
        else
        {
            _state = CONTROLLER;
            _subState = MIDI_CLOCK_OFF;

            _bpmLed.setState(LOW);          

            // display default message on screen
            _screenManager.printDefault(_currentPage, _bpm);
        
            // Reset the flag for further button events
            _wasPageSaved = 0;
        }
    }

    // save current page component's configuration and exits edit mode
    if (_editButton.pressedFor(PRESSED_FOR_WAIT))
    {   
        // stop sending MIDI Clock signal in case of the controller was sending such signal
        if (_state == CONTROLLER && _subState == MIDI_CLOCK_ON)
            sendMIDIRealTime(midi::Stop);        
        
        // saves the current page
        savePage(_currentPage); 
        _wasPageSaved = 1;        

        // prints the corresponding message and wait to continue 
        _screenManager.printSavedMessage();
        delay(2000);
        
        _state = CONTROLLER;
        _subState = MIDI_CLOCK_OFF;
    }
}
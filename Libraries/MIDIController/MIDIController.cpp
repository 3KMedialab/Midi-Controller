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
MIDIController::MIDIController(MidiWorker * midiWorker, IMIDIComponent ** midiComponents, uint8_t numMIDIComponents) 
{
    _midiWorker = midiWorker;
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
   _memoryManager.initialize(_midiComponents, _numMIDIComponents, _sequencer.getSequenceLength(), Step::getSize(), _globalConfig.getSize());
   _screenManager.initialize(I2C_ADDRESS, ROW_LENGTH, ROWS);

   // load from EEPROM the Global Configuration parameters
   _memoryManager.loadGlobalConfiguration(&_globalConfig);
   _wasGlobalConfigSaved = 0;
   _accesToGloabalEdit = 0;

   // load from EEPROM the default page of MIDI messages into the MIDI components
   _currentPage = 1;    
   _memoryManager.loadMIDIComponents(_currentPage, _midiComponents, _numMIDIComponents);
   _wasPageSaved = 0;
   
   // load from EEPROM the default sequence into the sequencer
   _sequencer.setCurrentSequence(1);
   _sequencer.loadCurrentSequence();
   _sequencer.setMidiWorker(_midiWorker);
   _wasSequenceSaved = 0;  
   _waitForStart = 0;
   _accesToSequencerEdit = 0;

   // set the default tempo
   _bpm = map(_selectValuePot.getSmoothValue(), 0, 1023, MIN_BPM, MAX_BPM);   

   // variable that controls the bpm led blink frequency
   _lastTimeBlink = 0;
   _lastTimeBlinkFade = 0;
   _lastTimeClock = 0;

   _screenManager.cleanScreen();
   _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn);

   // sequencer initialization
   _sequencer.setBpm(&_bpm);
   _sequencer.setMIDIChannel(_globalConfig.getSequencerMIDIChannel());

   // Controller initial state and substate
   _state = CONTROLLER;
   _subState = MIDI_CLOCK_OFF;

   // sending MIDI Clock is initialised to FALSE
   _isMIDIClockOn = 0;
   
   // wait for start flag is set to zero on controller initialization
   _waitForStart = 0;
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
                    _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn);
                }

            break;

            // display the previous MIDI message of the component loaded into the Screen Manager
            case EDIT_PAGE:
                
                if (_subState != DEFAULT_EDIT_MSG && _screenManager.getDisplayedMessageIndex() > 1)
                {
                    _screenManager.displayPreviousMIDIMsg();
                    _subState = EDIT_MIDI_TYPE;
                }

            break;

            // load the previous sequence from memory
            case SEQUENCER:
            
                if (_sequencer.getCurrentSequence() > 1)
                {
                    _sequencer.setCurrentSequence(_sequencer.getCurrentSequence() - 1);

                    // if playback is on controller must wait for the start of a new bar before load the new sequence into the sequencer
                    if (_sequencer.isPlayBackOn())
                    {
                        _waitForLoadSequence = 1;
                    }

                    else
                    {
                        _waitForLoadSequence = 0;
                        _sequencer.loadCurrentSequence();
                    }

                    _sequencer.printDefault();
                }

            break; 

            // display the previous step in the sequence
            case SEQUENCER_EDIT_STEP:

                _subState = SEQUENCER_EDIT_STEP_NOTE;
                _sequencer.printPreviousStep();                

            break;
        }
    }

    else
    {
        if (_waitForLoadSequence)
        {
            if (_syncManager.canStart())
            {
                _waitForLoadSequence = 0;
                _sequencer.loadCurrentSequence();
            }
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
                
                if (_currentPage < NUM_PAGES)
                {
                    _currentPage += 1;
                    loadPage(_currentPage);
                    _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn);
                }   
            
            break;
           
            // display the next MIDI message of the component loaded into the Screen Manager
            case EDIT_PAGE:
            
                if (_subState != DEFAULT_EDIT_MSG && (_screenManager.getDisplayedMessageIndex() < _screenManager.getDisplayedMIDIComponent()->getNumMessages()))
                {
                    _screenManager.displayNextMIDIMsg();
                    _subState = EDIT_MIDI_TYPE;
                }          
            
            break;

            // load the next sequence from memory
            case SEQUENCER:
                
                if (_sequencer.getCurrentSequence() < NUM_SEQUENCES)
                {
                    _sequencer.setCurrentSequence(_sequencer.getCurrentSequence() + 1);
                    
                    // if playback is on controller must wait for the start of a new bar before load the new sequence into the sequencer
                    if (_sequencer.isPlayBackOn())
                    {
                        _waitForLoadSequence = 1;
                    }

                    else
                    {
                        _waitForLoadSequence = 0;
                        _sequencer.loadCurrentSequence();
                    }

                    _sequencer.printDefault();
                }   
            
            break;

            // display the next step in the sequence
            case SEQUENCER_EDIT_STEP:

                _subState = SEQUENCER_EDIT_STEP_NOTE;
                _sequencer.printNextStep();                

            break;
        }
    }

    else
    {
        if (_waitForLoadSequence)
        {
            if (_syncManager.canStart())
            {
                _waitForLoadSequence = 0;
                _sequencer.loadCurrentSequence();
            }
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
                if (message->getType() != midi::InvalidType)
                {
                    _midiLed.setState(HIGH);
                    _midiWorker->sendMIDIMessage(message, _globalConfig.getMIDIChannel());                    
                    _midiLed.setState(LOW);
                } 
            }
        
            break;
        }
        
        // assign the MIDI messages information to the screen and display the first MIDI message
        case EDIT_PAGE:
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

        // assign the note of the selected MIDI component to the step currently being edited
        case SEQUENCER_EDIT_STEP:
        {
            if (_subState == SEQUENCER_EDIT_STEP_NOTE)
            {
                MIDIMessage * message = component->getMessageToSend();
            
                if (message != NULL)
                {
                    if (message->getType() == midi::NoteOn)
                    {
                        _sequencer.setDisplayedStepNote(message->getDataByte1());
                    } 
                }
            }

            break;
        }
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
* tempo or set the values on edit mode
*/
void MIDIController::processSelectValuePot()
{        
    if (_selectValuePot.wasChanged())
    {
        switch (_state)
        {
            // set controller's tempo and calculate delays for led blinking, MIDI clock signal sending and step sequence playing
            case CONTROLLER:
            case SEQUENCER:
            {
                _bpm = map(_selectValuePot.getSmoothValue(), 0, 1023, MIN_BPM, (MAX_BPM+1));

                // set the new bpm value into the sync manager and the step sequencer 
                _syncManager.setBpm(_bpm);              

                // calculate the step delay of the sequencer
                _sequencer.setBpm(&_bpm);

                // update screen regarding the state
                if (_state == CONTROLLER)
                {
                    _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn);
                }

                if (_state == SEQUENCER)
                {
                    _sequencer.printDefault();
                }
            
                break;
            } 

            // select a value for a midi message part
            case EDIT_PAGE:
            {
                // get the component displayed on screen currently
                IMIDIComponent * displayedComponent = _screenManager.getDisplayedMIDIComponent();             
                uint8_t displayedMessageIndex = (_screenManager.getDisplayedMessageIndex())-1;

                switch (_subState)
                {
                    case EDIT_MIDI_TYPE:    
                    {
                        // get the current MIDI message type
                        MIDIMessage oldMessage = displayedComponent->getMessages()[displayedMessageIndex];

                        // set the new MIDI message type into the component and refresh the screen
                        uint8_t * availableMIDIMessages = displayedComponent->getAvailableMessageTypes();    
                        uint8_t numAvailableMsgTypes = displayedComponent->getNumAvailableMessageTypes();                    
                       
                        displayedComponent->getMessages()[displayedMessageIndex].setType(availableMIDIMessages[map(_selectValuePot.getSmoothValue(),0,1023,0,numAvailableMsgTypes)]);

                        if (oldMessage.getType() != displayedComponent->getMessages()[displayedMessageIndex].getType())
                        {
                            _screenManager.refreshMIDIData();
                        }
                        break;
                    }

                    case EDIT_NOTE:
                    {
                        // set the new note value in to the component
                        uint8_t note = map(_selectValuePot.getSmoothValue(), 0, 1023, NOTE_C_1, NOTE_Cb8);

                        if (MIDIUtils::isNoteInScale(note, _globalConfig.getRootNote(), _globalConfig.getMode()))
                        {
                            displayedComponent->getMessages()[displayedMessageIndex].setDataByte1(note);

                            // print the new note value on the screen
                            _screenManager.refreshNoteValue(note);
                        }                                             
                   
                        break;
                    }

                    case EDIT_VELOCITY:
                    {

                        //set the new velocity value into the component
                        uint8_t velocity = map(_selectValuePot.getSmoothValue(), 0, 1023, 0, 128);
                        displayedComponent->getMessages()[displayedMessageIndex].setDataByte2(velocity);

                        // print the new velocity value on the screen
                        _screenManager.refreshVelocityValue(velocity);                          

                        break;
                    }

                    case EDIT_CC:
                    {
                        //set the new control change type value into the component
                        uint8_t ccValue = map(_selectValuePot.getSmoothValue(), 0, 1023, 0, 128);
                        displayedComponent->getMessages()[displayedMessageIndex].setDataByte1(ccValue);

                        // print the new cc value on the screen
                        _screenManager.refreshCCValue(ccValue);   
                        
                        break;
                    }       
                }

                break;
            }

            // select a value for a Global Configuration Parameter 
            case EDIT_GLOBAL_CONFIG:

                switch (_subState)
                {
                    // select the musical mode value
                    case EDIT_GLOBAL_MODE:
                    {
                        // get the current mode 
                        uint8_t currentMode = _globalConfig.getMode();

                        _globalConfig.setMode(map(_selectValuePot.getSmoothValue(),0,1023,MIDIUtils::Ionian,MIDIUtils::Chromatic + 1));

                        if (currentMode != _globalConfig.getMode())
                        {
                            _screenManager.refreshModeData(_globalConfig.getMode());
                        }

                        break;
                    }

                    // select the root note value
                    case EDIT_GLOBAL_ROOT_NOTE:
                    {
                        // get the current root note 
                        uint8_t currentRootNote = _globalConfig.getRootNote();

                        _globalConfig.setRootNote(map(_selectValuePot.getSmoothValue(),0,1023,MIDIUtils::C,MIDIUtils::B + 1));

                        if (currentRootNote != _globalConfig.getRootNote())
                        {
                            _screenManager.refreshRootNoteData(_globalConfig.getRootNote());
                        }

                        break;
                    }
                    
                    // select the MIDI channel value
                    case EDIT_GLOBAL_MIDI_CH:
                    {
                        // get the current root note 
                        uint8_t currentMIDIChannel = _globalConfig.getMIDIChannel();

                        _globalConfig.setMIDIChannel(map(_selectValuePot.getSmoothValue(),0,1023,MIDIUtils::CHANNEL1,MIDIUtils::CHANNEL16 + 1));

                        if (currentMIDIChannel != _globalConfig.getMIDIChannel())
                        {
                            _screenManager.refreshMIDIChannelData(_globalConfig.getMIDIChannel());
                        }

                        break;
                    }                   
                }
            
            break;

            // select a value for a step sequencer parameter 
            case SEQUENCER_EDIT_STEP:

                switch(_subState)
                {
                    // update the note assigned to the current edited step
                    case SEQUENCER_EDIT_STEP_NOTE:
                    {  
                        uint8_t note = map(_selectValuePot.getSmoothValue(), 0, 1023, NOTE_C_1, NOTE_Cb8);

                        if (MIDIUtils::isNoteInScale(note, _globalConfig.getRootNote(), _globalConfig.getMode()))
                        {
                            _sequencer.setDisplayedStepNote(note);                         
                        }

                        break;     
                    }

                    // update legato value assigned to the current edited step
                    case SEQUENCER_EDIT_STEP_LEGATO:
                    {
                        uint8_t legatoValue = map(_selectValuePot.getSmoothValue(), 0, 1023, 0, 2);
                        _sequencer.setDisplayedStepLegato(legatoValue);

                        break;
                    }

                    // update enabled value assigned to the current edited step
                    case SEQUENCER_EDIT_STEP_ENABLED:
                    {
                        uint8_t enabledValue = map(_selectValuePot.getSmoothValue(), 0, 1023, 0, 2);
                        _sequencer.setDisplayedStepEnabled(enabledValue);

                        break;
                    }
                }
                

            break;
        }  
    }
}

/*
* Process the button that controls MIDI clock when CONTROLLER mode is on, or move the cursor through the screen
* when EDIT mode is on, or star / stop sequencer playbak
*/
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

            case EDIT_PAGE:
                moveCursorToValue();                          
            break;

            case EDIT_GLOBAL_CONFIG:
                moveCursorToGLobalConfigParameter();                          
            break;
           
            case SEQUENCER:
                updateSequencerPlayBackStatus();                         
            break;

            case SEQUENCER_EDIT_STEP:
                moveCursorToStepValue();
            break;
        }    
    }
	
	// button was not pressed -> check whether start sending MIDI clock or start sequencer playback is pending
	else
	{
		if (_waitForStart)
		{
			switch (_state)
			{
				// activate/deactivate sending of MIDI clock signal
				case CONTROLLER:                
					updateMIDIClockState();
				break;
           
				// start/stop sequence playback
				case SEQUENCER:
					updateSequencerPlayBackStatus();                         
				break;
			}    
		}		
	}
}

/*
* Control the MIDI clock sending status
*/
void MIDIController::updateMIDIClockState()
{               
    if (!_waitForStart)
	{
		_subState == MIDI_CLOCK_OFF ? _subState=MIDI_CLOCK_ON : _subState=MIDI_CLOCK_OFF;  
	}

    switch (_subState)
    {
        // send start real time message
        case MIDI_CLOCK_ON:
			
			// check if sending MIDI clock can start
			if (_syncManager.canStart())
			{
				_isMIDIClockOn = 1;
				
				if (!_syncManager.isActive())
				{
					_syncManager.activate();
				}
				
				_waitForStart = 0;
				
				_midiWorker->sendMIDIStartClock();

                _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn); 
			}
			
			// sequencer is playing the sequence, so sending MIDI clock signal must wait for the start of the bar
			else
			{
				_waitForStart = 1;
			}
				
        break;

        // send stop realtime message
        case MIDI_CLOCK_OFF:
			
			// if MIDI clock signal was not being sent it's not necessary to send the MIDI Stop message
			if (!_waitForStart)
			{
				_midiWorker->sendMIDIStopClock();
			}
			
            _isMIDIClockOn = 0;
			_waitForStart = 0;
			
			// stop the sync manager and stop bpm indicator if controller is not sending MIDI Clock and sequencer is not playing the sequence
            if (!_sequencer.isPlayBackOn())
            {
                _midiLed.setState(LOW);
				_syncManager.deactivate();
            }

            _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn);
              
        break;
    }           
}

/*
* Activate/Deactivate the sequencer Playback
*/
void MIDIController::updateSequencerPlayBackStatus()
{    
	if (!_waitForStart)
	{
		_subState == PLAYBACK_OFF ? _subState=PLAYBACK_ON : _subState=PLAYBACK_OFF;    
	}
            
    switch (_subState)
    {
        // send start real time message
        case PLAYBACK_ON:
		
			// check if sequence playback can start
			if (_syncManager.canStart())
			{
				_sequencer.startPlayBack();
				
				if (!_syncManager.isActive())
				{
					_syncManager.activate();
				}
				
				_waitForStart = 0;
			}
			
			// MIDI clock signal is being sent, so playback start must wait for the start of the bar
			else
			{
				_waitForStart = 1;
			}
            
        break;

        case PLAYBACK_OFF:
		
            _sequencer.stopPlayBack();
			_waitForStart = 0;
            
            // stop the sync manager and stop bpm indicator if controller is not sending MIDI Clock and sequencer is not playing the sequence
            if (!_isMIDIClockOn)
            {
                _midiLed.setState(LOW);
				_syncManager.deactivate();
            }

            _sequencer.printDefault();

        break;        
    } 
}

/*
* Move the cursor to the different MIDI parameters while controller is on EDIT mode
*/
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

                    _subState = EDIT_MIDI_TYPE;
                    _screenManager.moveCursorToMsgType();

                break;                
            }
        
        break;

        case midi::ProgramChange:

            switch (_subState)
            {
                _subState = EDIT_MIDI_TYPE;
                _screenManager.moveCursorToMsgType();               
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
                
                    _subState = EDIT_MIDI_TYPE;
                    _screenManager.moveCursorToMsgType();
                
                break;
            }            
        break;

        case midi::InvalidType:
        break;
    }    
}    

/*
* Move the cursor to the different parameters on EDIT_GLOBAL_CONFIG mode
*/
void MIDIController::moveCursorToGLobalConfigParameter()
{
    switch (_subState)
    {
        case EDIT_GLOBAL_MODE:

            _subState = EDIT_GLOBAL_ROOT_NOTE;
            _screenManager.moveCursorToRootNote();

        break;

        case EDIT_GLOBAL_ROOT_NOTE:

            _subState = EDIT_GLOBAL_MIDI_CH;
            _screenManager.moveCursorToMIDIChannel();

        break;

        case EDIT_GLOBAL_MIDI_CH:
                
            _subState = EDIT_GLOBAL_MODE;
            _screenManager.moveCursorToMode();

        break;
    }
}   

void MIDIController::moveCursorToStepValue()
{
    switch (_subState)    {    

        case SEQUENCER_EDIT_STEP_NOTE:

            _subState = SEQUENCER_EDIT_STEP_LEGATO;
            _sequencer.moveCursorToLegato();

        break;

        case SEQUENCER_EDIT_STEP_LEGATO:
                
            _subState = SEQUENCER_EDIT_STEP_ENABLED;
            _sequencer.moveCursorToEnabled();
                
        break;

        case SEQUENCER_EDIT_STEP_ENABLED:
                
            _subState = SEQUENCER_EDIT_STEP_NOTE;
            _sequencer.moveCursorToNote();
                
        break;
    }
}  

/*
* Send a MIDI clock tick and blink the LED
*/
void MIDIController::sendMIDIClock()
{   
    // send MIDI clock signal regarding the tempo
    if (_syncManager.getSyncTimeStamp() - _lastTimeClock >= ((MICROSECONDS_PER_MINUTE / _bpm) / 24))
    {                       
        if (_isMIDIClockOn)
        {    
            _midiWorker->sendMIDIClock();
            _lastTimeClock = _syncManager.getSyncTimeStamp();
        }    
    }   
}

/*
* Playback current sequence assigned to the sequencer
*/
void MIDIController::playBackSequence()
{
    // play next step in the sequence if sequencer playback is on
    if (_sequencer.playBackSequence(_syncManager.getSyncTimeStamp()))
    {
        // update screen with next step data if controller state is SEQUENCER
        if (_state == SEQUENCER && _subState == PLAYBACK_ON)
        {
            _sequencer.updateDisplayedStep();
        }
    }    
}

/*
* Update led bpm status
*/
void MIDIController::updateBpmIndicatorStatus()
{
	if (_waitForStart || _waitForLoadSequence)
	{
		if (_syncManager.getSyncTimeStamp() - _lastTimeBlinkFade >= ((MICROSECONDS_PER_MINUTE / _bpm) / 8))
		{
			_midiLed.setState(!_midiLed.getState());
            _lastTimeBlinkFade = _syncManager.getSyncTimeStamp();
		}
	}
	
	else
	{
		if (_syncManager.getSyncTimeStamp() - _lastTimeBlink >= ((MICROSECONDS_PER_MINUTE / _bpm) / 2))
		{
			if (_isMIDIClockOn || _sequencer.isPlayBackOn())
			{          
				_midiLed.setState(!_midiLed.getState());
				_lastTimeBlink = _syncManager.getSyncTimeStamp();
			}
			
			else
			{
				_midiLed.setState(LOW);
			}			
		}		
	}    
}

/*
* Process the button that set the mode operation: MIDI Controller or Sequencer
*/
void MIDIController::processOperationModeButton()
{
    _operationModeButton.read();

    if (_operationModeButton.wasPressed())
    {
        switch (_state)
        {
            // Set the operation mode to Sequencer
            case CONTROLLER:
                _state = SEQUENCER;     
                _subState = _sequencer.isPlayBackOn() ? PLAYBACK_ON : PLAYBACK_OFF;  
                _sequencer.printDefault();

            break;

            case SEQUENCER:
                 _state = CONTROLLER;             
                 _subState = _isMIDIClockOn ? MIDI_CLOCK_ON : MIDI_CLOCK_OFF;
                 _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn); 

            break;
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
        if (!_wasPageSaved && !_wasGlobalConfigSaved && !_wasSequenceSaved)
        {
            switch (_state)
            {
                // defaut edit message is displayed on screen
                case CONTROLLER:                     
    
                    _state = EDIT_PAGE;
                    _subState = DEFAULT_EDIT_MSG;                        
                  
                    _screenManager.printSelectComponentMessage();
                
                break;
    
                // Enter in edit page components mode: LED doesn't blink, default message is displayed on screen 
                case EDIT_PAGE:
    
                    _state = CONTROLLER;
                    _subState = _isMIDIClockOn ? MIDI_CLOCK_ON : MIDI_CLOCK_OFF; 

                    _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn);       
                    
                break;

                // Enter in global config edit mode: LED doesn't blink, default message is displayed on screen 
                case EDIT_GLOBAL_CONFIG:

                    if (_accesToGloabalEdit)
                        _accesToGloabalEdit = 0;
                    
                    else
                    {
                        _state = CONTROLLER;
                        _subState = _isMIDIClockOn ? MIDI_CLOCK_ON : MIDI_CLOCK_OFF;                
                              
                        _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn);     
                    }
                    
                break;

                // defaut edit message is displayed on screen
                case SEQUENCER:                     
    
                    _state = SEQUENCER_EDIT_STEP;
                    _subState = SEQUENCER_EDIT_STEP_NOTE;                       
                  
                    _sequencer.printEditStepData();
                
                break;

                case SEQUENCER_EDIT_STEP:
    
                    _state = SEQUENCER;
                    _subState = _sequencer.isPlayBackOn() ? PLAYBACK_ON : PLAYBACK_OFF; 

                    _sequencer.printDefault();       
                    
                break;

                case SEQUENCER_EDIT_CONFIG:

                    if (_accesToSequencerEdit)
                        _accesToSequencerEdit = 0;
                    
                    else
                    {
                        _state = SEQUENCER;
                        _subState = _sequencer.isPlayBackOn() ? PLAYBACK_ON : PLAYBACK_OFF; 

                        _sequencer.printDefault();  
                    }
                    
                break;                     
            }        
        }           
   
        // after saving a page or global configuration, set controller status to CONTROLLER
        else
        {
            switch (_state)
            {
                case CONTROLLER:

                    // display default message on screen
                    _screenManager.printDefault(_currentPage, NUM_PAGES, _bpm, _globalConfig, _isMIDIClockOn);
        
                    // Reset the flags for further button events
                    _wasPageSaved = 0;
                    _wasGlobalConfigSaved = 0;

                break;

                case SEQUENCER:

                    _sequencer.printDefault();

                    // Reset the flags for further button events
                    _wasSequenceSaved = 0;

                break;
            }
        }
    }

    // save current page component's configuration / global configuration and exits edit mode
    if (_editButton.pressedFor(PRESSED_FOR_WAIT))
    {   
        switch (_state)
        {
            case CONTROLLER:

                _state = EDIT_GLOBAL_CONFIG;
                _subState = EDIT_GLOBAL_MODE;    

                _accesToGloabalEdit = 1;               

                _screenManager.printEditGlobalConfig(_globalConfig);      

            break;

            case SEQUENCER:

                _state = SEQUENCER_EDIT_CONFIG;
                _subState = SEQUENCER_EDIT_PLAYBACK_MODE;    

                _accesToSequencerEdit = 1;               

                _sequencer.printEditConfig(_globalConfig);                      

            break;    

            case EDIT_PAGE:

                _isMIDIClockOn = 0;
                _sequencer.stopPlayBack();
                _syncManager.deactivate();
                _midiLed.setState(LOW);     

                // saves the current page
                savePage(_currentPage); 
                _wasPageSaved = 1;        

                // prints a message and waits to continue 
                _screenManager.printSavedMessage();
                delay(2000);      

                _state = CONTROLLER;
                _subState = MIDI_CLOCK_OFF;           
            
            break;

            case EDIT_GLOBAL_CONFIG:

                if (!_accesToGloabalEdit)
                {
                    _isMIDIClockOn = 0;
                    _sequencer.stopPlayBack();
                    _syncManager.deactivate();
                    _midiLed.setState(LOW);  

                    // saves the global configuration parameters
                    _memoryManager.saveGlobalConfiguration(_globalConfig); 
                    _wasGlobalConfigSaved = 1;        

                    // prints a message and waits to continue 
                    _screenManager.printSavedMessage();
                    delay(2000);

                    _state = CONTROLLER;
                    _subState = MIDI_CLOCK_OFF;
                }                 

            break;

            case SEQUENCER_EDIT_STEP:

                _isMIDIClockOn = 0;
                _sequencer.stopPlayBack();
                _syncManager.deactivate();
                _midiLed.setState(LOW);     

                // saves the current page
                _sequencer.saveCurrentSequence(); 
                _wasSequenceSaved = 1;        

                // prints a message and waits to continue 
                _screenManager.printSavedMessage();
                delay(2000);      

                _state = SEQUENCER;
                _subState = PLAYBACK_OFF;     

            break;

            case SEQUENCER_EDIT_CONFIG:

                if (!_accesToSequencerEdit)
                {
                    _isMIDIClockOn = 0;
                    _sequencer.stopPlayBack();
                    _syncManager.deactivate();
                    _midiLed.setState(LOW);  

                    // saves the global configuration parameters
                    _memoryManager.saveGlobalConfiguration(_globalConfig); 
                    _wasGlobalConfigSaved = 1;        

                    // prints a message and waits to continue 
                    _screenManager.printSavedMessage();
                    delay(2000);

                    _state = SEQUENCER;
                    _subState = PLAYBACK_OFF;
                }

            break;
        }            
    }
}

/*
* Update the status of the synchronisation maanager
*/
void MIDIController::updateSyncStatus()
{
    _syncManager.updateSyncStatus();
}
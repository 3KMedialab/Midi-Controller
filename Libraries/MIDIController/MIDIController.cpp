/*
 * MIDIController.h
 *
 * The MIDI Controller class. It contains an array of MIDIButtons and MIDIPotentiometers. It is the core class within the system.
 * The controller also manages the behaviour of the step sequencer. 
 *
 * The configuration of the controller is stored in the ControllerConfig.h file
 *
 * Copyright 2018 3K MEDIALAB
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
* midiWorker: the MIDI interface that the controller will use.
* midiComponents: the array of MIDI components the controller will manage.
* numMIDIComponents: number of MIDI components assigned to the controller
*/
MIDIController::MIDIController(MidiWorker *midiWorker, IMIDIComponent **midiComponents, uint8_t numMIDIComponents)
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
MIDIController::MIDIController(IMIDIComponent **midiComponents, uint8_t numMIDIComponents)
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
    _screenManager.initialize();

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
    _accesToSequencerEdit = 0;

    // set the default tempo
    _syncManager.setBpm(map(_selectValuePot.getSmoothValue(), 0, 1023, MIN_BPM, MAX_BPM + 1));

    // Display the controller current page, bpm and MIDI clock status data
    _screenManager.printDefault(_currentPage, NUM_PAGES, _syncManager.getBpm(), _isMIDIClockOn);

    // sequencer initialization
    _sequencer.setMIDIChannel(_globalConfig.getSequencerMIDIChannel());

    // Controller initial state and substate
    _state = CONTROLLER;
    _subState = MIDI_CLOCK_OFF;

    // sending MIDI Clock is initialised to FALSE
    _isMIDIClockOn = 0;
    _resetMIDIClockPeriod = 0;
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
                _screenManager.printDefault(_currentPage, NUM_PAGES, _syncManager.getBpm(), _isMIDIClockOn);
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
                _sequencer.loadCurrentSequence();
                _sequencer.printDefault(_syncManager);
            }

            break;

        // display the previous step in the sequence
        case SEQUENCER_EDIT_STEP:

            _subState = SEQUENCER_EDIT_STEP_NOTE;
            _sequencer.printPreviousStep();

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

            if (_currentPage < NUM_PAGES)
            {
                _currentPage += 1;
                loadPage(_currentPage);
                _screenManager.printDefault(_currentPage, NUM_PAGES, _syncManager.getBpm(), _isMIDIClockOn);
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
                _sequencer.loadCurrentSequence();
                _sequencer.printDefault(_syncManager);
            }

            break;

        // display the next step in the sequence
        case SEQUENCER_EDIT_STEP:

            _subState = SEQUENCER_EDIT_STEP_NOTE;
            _sequencer.printNextStep();

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
void MIDIController::processMidiComponent(IMIDIComponent *component)
{
    // edit mode is off: send MIDI message
    switch (_state)
    {
    // send a MIDI message
    case CONTROLLER:
    case SEQUENCER:
    {
        MIDIMessage *message = component->getMessageToSend();

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
            MIDIMessage *message = component->getMessageToSend();

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
            // set the new bpm value into the sync manager
            _syncManager.setBpm(map(_selectValuePot.getSmoothValue(), 0, 1023, MIN_BPM, MAX_BPM + 1));

            // update screen regarding the state
            if (_state == CONTROLLER)
            {
                _screenManager.printDefault(_currentPage, NUM_PAGES, _syncManager.getBpm(), _isMIDIClockOn);
            }

            if (_state == SEQUENCER)
            {
                _sequencer.printDefault(_syncManager);
            }

            break;
        }

        // select a value for a midi message part
        case EDIT_PAGE:
        {
            // get the component displayed on screen currently
            IMIDIComponent *displayedComponent = _screenManager.getDisplayedMIDIComponent();
            uint8_t displayedMessageIndex = (_screenManager.getDisplayedMessageIndex()) - 1;

            switch (_subState)
            {
            case EDIT_MIDI_TYPE:
            {
                // get the current MIDI message type
                MIDIMessage oldMessage = displayedComponent->getMessages()[displayedMessageIndex];

                // set the new MIDI message type into the component and refresh the screen
                uint8_t *availableMIDIMessages = displayedComponent->getAvailableMessageTypes();
                uint8_t numAvailableMsgTypes = displayedComponent->getNumAvailableMessageTypes();

                displayedComponent->getMessages()[displayedMessageIndex].setType(availableMIDIMessages[map(_selectValuePot.getSmoothValue(), 0, 1023, 0, numAvailableMsgTypes)]);

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
                uint8_t velocity = map(_selectValuePot.getSmoothValue(), 0, 1023, 1, 128);
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

                _globalConfig.setMode(map(_selectValuePot.getSmoothValue(), 0, 1023, MIDIUtils::Ionian, MIDIUtils::Chromatic + 1));

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

                _globalConfig.setRootNote(map(_selectValuePot.getSmoothValue(), 0, 1023, MIDIUtils::C, MIDIUtils::B + 1));

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

                _globalConfig.setMIDIChannel(map(_selectValuePot.getSmoothValue(), 0, 1023, MIDIUtils::CHANNEL1, MIDIUtils::CHANNEL16 + 1));

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

            switch (_subState)
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

        // select a value for a Sequencer Configuration Parameter
        case SEQUENCER_EDIT_CONFIG:

            switch (_subState)
            {
            // select the sequencer playback mode
            case SEQUENCER_EDIT_PLAYBACK_MODE:
            {
                // get the current playback mode
                uint8_t currentMode = _sequencer.getPlayBackMode();

                _sequencer.setPlayBackMode(map(_selectValuePot.getSmoothValue(), 0, 1023, 0, _sequencer.getPlayBackModeTypesNumber()));

                if (currentMode != _sequencer.getPlayBackMode())
                {
                    if (_sequencer.isPlayBackOn())
                    {
                        _sequencer.stopNote();
                    }

                    _sequencer.refreshDisplayedPlayBackMode(_sequencer.getPlayBackMode());
                }

                break;
            }
            // activate/deactivate send clock in sync with sequencer playback
            case SEQUENCER_EDIT_SEND_CLOCK_WHILE_PLAYBACK:
            {
                if (!_sequencer.isPlayBackOn())
                {

                    uint8_t currentSendClockWhilePlayBack = _globalConfig.getSendClockWhilePlayback();
                    _globalConfig.setSendClockWhilePlayback(map(_selectValuePot.getSmoothValue(), 0, 1023, 0, 2));

                    if (currentSendClockWhilePlayBack != _globalConfig.getSendClockWhilePlayback())
                    {
                        _sequencer.refreshDisplayedSendClockWhilePlayback(_globalConfig.getSendClockWhilePlayback());
                    }
                }

                break;
            }

            // select the sequencer step size
            case SEQUENCER_EDIT_STEP_SIZE:
            {
                //sequencer step size cannot be changed while playback is on
                if (!_sequencer.isPlayBackOn())
                {
                    uint8_t currentStepSize = _sequencer.getStepSize();

                    // set the new value of the step size if valid
                    uint8_t newStepSize = map(_selectValuePot.getSmoothValue(), 0, 1023, Sequencer::QUARTER, Sequencer::THIRTYSECOND + 1);

                    if (_sequencer.isStepSizeValueValid(newStepSize))
                    {
                        _sequencer.setStepSize(newStepSize);

                        if (currentStepSize != _sequencer.getStepSize())
                        {
                            _sequencer.refreshDisplayedStepSizeValue(_sequencer.getStepSize());
                        }
                    }
                }

                break;
            }

            // select the sequencer MIDI channel value
            case SEQUENCER_EDIT_MIDI_CH:
            {
                uint8_t currentSeqMIDIChannel = _globalConfig.getSequencerMIDIChannel();

                _globalConfig.setSequencerMIDIChannel(map(_selectValuePot.getSmoothValue(), 0, 1023, MIDIUtils::CHANNEL1, MIDIUtils::CHANNEL16 + 1));

                if (currentSeqMIDIChannel != _globalConfig.getSequencerMIDIChannel())
                {
                    _sequencer.refreshDisplayedMIDIChannel(_globalConfig.getSequencerMIDIChannel());
                }

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

        case SEQUENCER_EDIT_CONFIG:
            moveCursorToSequencerConfigParameter();
            break;
        }
    }
}

/*
* Control the MIDI clock sending status
*/
void MIDIController::updateMIDIClockState()
{
    // if sequencer playback is on, MIDI clock cannot be activated/deactivated
    if (!_sequencer.isPlayBackOn())
    {
        _subState == MIDI_CLOCK_OFF ? _subState = MIDI_CLOCK_ON : _subState = MIDI_CLOCK_OFF;

        switch (_subState)
        {
        // send start real time message
        case MIDI_CLOCK_ON:
            _isMIDIClockOn = 1;
            _midiWorker->sendMIDIStartClock();
            break;

        // send stop realtime message
        case MIDI_CLOCK_OFF:
            _midiWorker->sendMIDIStopClock();
            _isMIDIClockOn = 0;
            break;
        }

        _screenManager.printDefault(_currentPage, NUM_PAGES, _syncManager.getBpm(), _isMIDIClockOn);
    }
}

/*
* Activate/Deactivate the sequencer Playback
*/
void MIDIController::updateSequencerPlayBackStatus()
{
    // sequencer playback is activated/deactivated either when MIDI clock is off or MIDI clock is and is played in sync with the sequencer
    if (!_isMIDIClockOn || (_isMIDIClockOn && _globalConfig.getSendClockWhilePlayback() && _sequencer.isPlayBackOn()))
    {
        _subState == PLAYBACK_OFF ? _subState = PLAYBACK_ON : _subState = PLAYBACK_OFF;

        switch (_subState)
        {
        // start sequence playback
        case PLAYBACK_ON:
            _sequencer.startPlayBack();
            _resetMIDIClockPeriod = 1;

            // start MIDI clock in sync regarding global configuration
            if (_globalConfig.getSendClockWhilePlayback())
            {
                _midiWorker->sendMIDIStartClock();
                _isMIDIClockOn = 1;
            }

            break;

        case PLAYBACK_OFF:
            _sequencer.stopPlayBack();

            // stop sequencer playback
            if (_globalConfig.getSendClockWhilePlayback())
            {
                _midiWorker->sendMIDIStopClock();
                _isMIDIClockOn = 0;
            }

            break;
        }

        _sequencer.printDefault(_syncManager);
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

/*
* Moves the cursor to the different step parameters in step edit screen
*/
void MIDIController::moveCursorToStepValue()
{
    switch (_subState)
    {

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
* Moves the cursor to the different sequencer global configuration parameters in global configuration screen
*/
void MIDIController::moveCursorToSequencerConfigParameter()
{
    switch (_subState)
    {
    case SEQUENCER_EDIT_PLAYBACK_MODE:

        _subState = SEQUENCER_EDIT_SEND_CLOCK_WHILE_PLAYBACK;
        _sequencer.moveCursorToSendClockWhilePlayback();

        break;

    case SEQUENCER_EDIT_SEND_CLOCK_WHILE_PLAYBACK:

        _subState = SEQUENCER_EDIT_STEP_SIZE;
        _sequencer.moveCursorToStepSize();

        break;

    case SEQUENCER_EDIT_STEP_SIZE:

        _subState = SEQUENCER_EDIT_MIDI_CH;
        _sequencer.moveCursorToMIDIChannel();

        break;

    case SEQUENCER_EDIT_MIDI_CH:

        _subState = SEQUENCER_EDIT_PLAYBACK_MODE;
        _sequencer.moveCursorToPlayBackMode();

        break;
    }
}

/*
* Send a MIDI clock tick. This method is called from the interrupt method set to the timer1 interrupt
*/
void MIDIController::sendMIDIClock()
{
    // send MIDI clock signal regarding the tempo
    if (_isMIDIClockOn)
    {
        _midiWorker->sendMIDIClock();
    }
}

/*
* Playback current sequence assigned to the sequencer. This method is called from the interrupt method set to the timer1 interrupt
*/
void MIDIController::playBackSequence()
{
    // play next step in the sequence if sequencer playback is on
    _sequencer.playBackSequence();
}

/*
* Update led bpm status
*/
void MIDIController::updateBpmIndicatorStatus()
{
    if (_isMIDIClockOn || _sequencer.isPlayBackOn())
    {
        _midiLed.setState(!_midiLed.getState());
    }

    else
    {
        _midiLed.setState(LOW);
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
            _sequencer.printDefault(_syncManager);

            break;
        
        // Set the operation mode to Controller
        case SEQUENCER:
            _state = CONTROLLER;
            _subState = _isMIDIClockOn ? MIDI_CLOCK_ON : MIDI_CLOCK_OFF;
            _screenManager.printDefault(_currentPage, NUM_PAGES, _syncManager.getBpm(), _isMIDIClockOn);

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

                _midiLed.setState(HIGH);  

                _screenManager.printSelectComponentMessage();

                break;

            // Enter in edit page components mode: LED doesn't blink, default message is displayed on screen
            case EDIT_PAGE:

                _state = CONTROLLER;
                _subState = _isMIDIClockOn ? MIDI_CLOCK_ON : MIDI_CLOCK_OFF;

                _midiLed.setState(LOW);  

                _screenManager.printDefault(_currentPage, NUM_PAGES, _syncManager.getBpm(), _isMIDIClockOn);

                break;

            // Enter in global config edit mode: default message is displayed on screen
            case EDIT_GLOBAL_CONFIG:

                if (_accesToGloabalEdit)              
                    _accesToGloabalEdit = 0;                   
          
                else
                {
                    _state = CONTROLLER;
                    _subState = _isMIDIClockOn ? MIDI_CLOCK_ON : MIDI_CLOCK_OFF;

                    _midiLed.setState(LOW);  

                    _screenManager.printDefault(_currentPage, NUM_PAGES, _syncManager.getBpm(), _isMIDIClockOn);
                }

                break;

            // display step edit screen
            case SEQUENCER:

                _state = SEQUENCER_EDIT_STEP;
                _subState = SEQUENCER_EDIT_STEP_NOTE;

                _midiLed.setState(HIGH);  

                _sequencer.printEditStepData();

                break;

            // display sequencer default screen
            case SEQUENCER_EDIT_STEP:

                _state = SEQUENCER;
                _subState = _sequencer.isPlayBackOn() ? PLAYBACK_ON : PLAYBACK_OFF;

                _midiLed.setState(LOW);  

                _sequencer.printDefault(_syncManager);

                break;

            // display sequencer global configuration screen
            case SEQUENCER_EDIT_CONFIG:

                if (_accesToSequencerEdit)              
                    _accesToSequencerEdit = 0;                  

                else
                {
                    _state = SEQUENCER;
                    _subState = _sequencer.isPlayBackOn() ? PLAYBACK_ON : PLAYBACK_OFF;

                    _midiLed.setState(LOW);  

                    _sequencer.printDefault(_syncManager);
                }

                break;
            }
        }

        // after saving a page or global configuration
        else
        {
            switch (_state)
            {
            case CONTROLLER:

                // display default message on screen
                _screenManager.printDefault(_currentPage, NUM_PAGES, _syncManager.getBpm(), _isMIDIClockOn);

                // Reset the flags for further button events
                _wasPageSaved = 0;
                _wasGlobalConfigSaved = 0;

                break;

            case SEQUENCER:

                _sequencer.printDefault(_syncManager);

                // Reset the flags for further button events
                _wasSequenceSaved = 0;
                _wasGlobalConfigSaved = 0;

                break;
            }

            _midiLed.setState(LOW);  
        }
    }

    // save current page component's configuration / steps within a sequence / global configuration and exits edit mode
    if (_editButton.pressedFor(PRESSED_FOR_WAIT))
    {
        switch (_state)
        {
        case CONTROLLER:

            _state = EDIT_GLOBAL_CONFIG;
            _subState = EDIT_GLOBAL_MODE;

            _accesToGloabalEdit = 1;

            _midiLed.setState(HIGH);  

            _screenManager.printEditGlobalConfig(_globalConfig);

            break;

        case SEQUENCER:

            _state = SEQUENCER_EDIT_CONFIG;
            _subState = SEQUENCER_EDIT_PLAYBACK_MODE;

            _accesToSequencerEdit = 1;

            _midiLed.setState(HIGH);  

            _sequencer.printEditConfig(_globalConfig);

            break;

        case EDIT_PAGE:

            if (_isMIDIClockOn)
            {
                _isMIDIClockOn = 0;
                _midiWorker->sendMIDIStopClock();
            }

            if (_sequencer.isPlayBackOn())
            {
                _sequencer.stopPlayBack();
            }

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
                if (_isMIDIClockOn)
                {
                    _isMIDIClockOn = 0;
                    _midiWorker->sendMIDIStopClock();
                }

                if (_sequencer.isPlayBackOn())
                {
                    _sequencer.stopPlayBack();
                }
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

            if (_isMIDIClockOn)
            {
                _isMIDIClockOn = 0;
                _midiWorker->sendMIDIStopClock();
            }

            if (_sequencer.isPlayBackOn())
            {
                _sequencer.stopPlayBack();
            }

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
                if (_isMIDIClockOn)
                {
                    _isMIDIClockOn = 0;
                    _midiWorker->sendMIDIStopClock();
                }

                if (_sequencer.isPlayBackOn())
                {
                    _sequencer.stopPlayBack();
                }

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

uint16_t MIDIController::getBpm()
{
    return _syncManager.getBpm();
}

uint8_t MIDIController::getStepSize()
{
    return _sequencer.getStepSize();
}

uint8_t MIDIController::getResetMIDIClockPeriod()
{
    return _resetMIDIClockPeriod;
}

void MIDIController::setResetMIDIClockPeriod(uint8_t resetMIDIClockPeriod)
{
    _resetMIDIClockPeriod = resetMIDIClockPeriod;
}


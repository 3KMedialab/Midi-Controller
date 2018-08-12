/*
 * Sequencer.h
 *
 * Step sequencer implementation allowing three different playback modes (Forward, Backward and Random) and different step sizes (1/4, 1/8, 1/16 and 1/32) 
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
#include "Sequencer.h"

Sequencer::Sequencer(uint8_t mode, uint8_t stepSize, MemoryManager *memoryManager, ScreenManager *screenManager)
{
    _playBackOn = 0;
    _stepSize = stepSize;
    _playBackMode = mode;
    _currentSequence = 1;
    _stopCurrentPlayedNote = 0;
    _loadNewSequence = 0;

    switch (_playBackMode)
    {
    case FORWARD:
    case RANDOM:
        _playBackStep = 0;
        break;

    case BACKWARD:
        _playBackStep = LENGTH - 1;
        break;
    }

    _memoryManager = memoryManager;
    _screenManager = screenManager;
}

/*
* SETTER METHODS
*/

void Sequencer::setPlayBackStep(int8_t currentStep)
{
    _playBackStep = currentStep;
}

void Sequencer::setMIDIChannel(uint8_t channel)
{
    _midiChannel = channel;
}

void Sequencer::setPlayBackMode(uint8_t mode)
{
    _playBackMode = mode;

    if (!_playBackOn)
    {
        switch (_playBackMode)
        {
        case FORWARD:
        case RANDOM:
            _playBackStep = 0;
            break;

        case BACKWARD:
            _playBackStep = LENGTH - 1;
            break;
        }
    }
}

void Sequencer::setStepSize(uint8_t size)
{
    _stepSize = size;
}

void Sequencer::setCurrentSequence(uint8_t numSequence)
{
    _currentSequence = numSequence;
}

void Sequencer::setMidiWorker(MidiWorker *midiWorker)
{
    _midiWorker = midiWorker;
}

void Sequencer::setDisplayedStepNote(uint8_t note)
{
    _steps[_screenManager->getDisplayedStepNumber() - 1].setNote(note);

    // refresh note value on screen
    _screenManager->refreshStepNoteValue(note);
}

void Sequencer::setDisplayedStepLegato(uint8_t legato)
{
    _steps[_screenManager->getDisplayedStepNumber() - 1].setLegato(legato);

    // refresh legato value on screen
    _screenManager->refreshStepLegatoValue(legato);
}

void Sequencer::setDisplayedStepEnabled(uint8_t enabled)
{
    _steps[_screenManager->getDisplayedStepNumber() - 1].setEnabled(enabled);

    // refresh enabled value on screen
    _screenManager->refreshStepEnabledValue(enabled);
}

/*
* GETTER METHODS
*/

int8_t Sequencer::getPlayBackStep()
{
    return _playBackStep;
}

uint8_t Sequencer::isPlayBackOn()
{
    return _playBackOn;
}

uint8_t Sequencer::isStepSizeValueValid(uint8_t stepSizeValue)
{
    switch (stepSizeValue)
    {
    case QUARTER:
    case EIGHTH:
    case SIXTEENTH:
    case THIRTYSECOND:
        return 1;
    default:
        return 0;
    }
}

uint8_t Sequencer::getPlayBackMode()
{
    return _playBackMode;
}

uint8_t Sequencer::getPlayBackModeTypesNumber()
{
    return PLAYBACK_MODE_TYPES;
}

uint8_t Sequencer::getMIDIChannel()
{
    return _midiChannel;
}

uint8_t Sequencer::getSequenceLength()
{
    return LENGTH;
}

Step *Sequencer::getSequence()
{
    return _steps;
}

Step Sequencer::getSequenceStep(uint8_t pos)
{
    return _steps[pos];
}

uint8_t Sequencer::getStepSize()
{
    return _stepSize;
}

uint8_t Sequencer::getCurrentSequence()
{
    return _currentSequence;
}

/*
* Load a new sequence from EEPROM into the sequencer
*/
void Sequencer::loadCurrentSequence()
{
    // new sequence is loaded in sync when playback mode is on
    if (isPlayBackOn())
    {
        _loadNewSequence = 1;
    }

    else
    {
        _memoryManager->loadSequence(_currentSequence, _steps, LENGTH);
    }
}

/*
* Stores current sequence into EEPROM
*/
void Sequencer::saveCurrentSequence()
{
    _memoryManager->saveSequence(_currentSequence, _steps, LENGTH);
}

/*
* Start sequencer playback
*/
void Sequencer::startPlayBack()
{
    _playBackOn = 1;
}

/*
* Stop playback and reset first step to be played
*/
void Sequencer::stopPlayBack()
{
    stopCurrentNotePlayed();

    switch (_playBackMode)
    {
    case FORWARD:
    case RANDOM:
        _playBackStep = 0;
        break;

    case BACKWARD:
        _playBackStep = LENGTH - 1;
        break;
    }

    _playBackOn = 0;
}

void Sequencer::stopNote()
{
    _stopCurrentPlayedNote = 1;
}

/*
* Plays next step within a sequence regarding the selected playback mode
*/
uint8_t Sequencer::playBackSequence()
{
    if (_playBackOn)
    {
        // when playback mode has changed, it's necessary to stop current played note before play next step
        if (_stopCurrentPlayedNote)
        {
            stopCurrentNotePlayed();
        }

        // if sequencer has to load a new sequence while playback is on
        if (_loadNewSequence)
        {
            stopCurrentNotePlayed();
            _memoryManager->loadSequence(_currentSequence, _steps, LENGTH);
            _loadNewSequence = 0;
        }

        // plays next step
        switch (_playBackMode)
        {
        case FORWARD:
            playBackForward();
            break;

        case BACKWARD:
            playBackBackward();
            break;

        case RANDOM:
            playBackRandom();
            break;
        }

        return 1; // play next step is true
    }

    return 0;
}

/*
* Calculate and play next step when Forward mode is selected
*/
void Sequencer::playBackForward()
{
    MIDIMessage msgNoteOff;
    MIDIMessage msgNoteOn;
    uint8_t isStepLegato;

    // stop last played step
    msgNoteOff.setType(midi::NoteOff);

    // last step within a sequence
    if (_playBackStep == 0)
    {
        msgNoteOff.setDataByte1(_steps[LENGTH - 1].getNote());
        isStepLegato = _steps[LENGTH - 1].isLegato();
    }

    else
    {
        msgNoteOff.setDataByte1(_steps[_playBackStep - 1].getNote());
        isStepLegato = _steps[_playBackStep - 1].isLegato();
    }

    // send note off message if legato is off
    if (!isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);
    }

    // Play current step if enabled
    if (_steps[_playBackStep].isEnabled())
    {
        // set current note being played
        _currentNotePlayed = _steps[_playBackStep].getNote();

        msgNoteOn.setType(midi::NoteOn);
        msgNoteOn.setDataByte1(_steps[_playBackStep].getNote());
        msgNoteOn.setDataByte2(127);
        _midiWorker->sendMIDIMessage(&msgNoteOn, _midiChannel);
    }

    // send note off message if legato is on
    if (isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);
    }

    // Move sequence to next step
    _playBackStep++;

    if (_playBackStep >= LENGTH)
    {
        _playBackStep = 0;
    }
}

/*
* Calculate and play next step when Backward mode is selected
*/
void Sequencer::playBackBackward()
{
    MIDIMessage msgNoteOff;
    MIDIMessage msgNoteOn;
    uint8_t isStepLegato;

    // stop last played step
    msgNoteOff.setType(midi::NoteOff);

    // last step of the sequence
    if (_playBackStep == LENGTH - 1)
    {
        msgNoteOff.setDataByte1(_steps[0].getNote());
        isStepLegato = _steps[0].isLegato();
    }

    else
    {
        msgNoteOff.setDataByte1(_steps[_playBackStep + 1].getNote());
        isStepLegato = _steps[_playBackStep + 1].isLegato();
    }

    // send note off message if legato is off
    if (!isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);
    }

    // Play current step if enabled
    if (_steps[_playBackStep].isEnabled())
    {
        // set current note being played
        _currentNotePlayed = _steps[_playBackStep].getNote();

        msgNoteOn.setType(midi::NoteOn);
        msgNoteOn.setDataByte1(_steps[_playBackStep].getNote());
        msgNoteOn.setDataByte2(127);
        _midiWorker->sendMIDIMessage(&msgNoteOn, _midiChannel);
    }

    // send note off message if legato is on
    if (isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);
    }

    // Move sequence to next step
    _playBackStep--;

    if (_playBackStep < 0)
    {
        _playBackStep = LENGTH - 1;
    }
}

/*
* Calculate and play next step when Random mode is selected
*/
void Sequencer::playBackRandom()
{
    MIDIMessage msgNoteOff;
    MIDIMessage msgNoteOn;
    uint8_t isStepLegato;

    // stop last played step
    msgNoteOff.setType(midi::NoteOff);
    msgNoteOff.setDataByte1(_steps[_playBackStep].getNote());

    isStepLegato = _steps[_playBackStep].isLegato();

    // send note off message when legato is off
    if (!isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);
    }

    // Calculate next step
    _playBackStep = random(0, LENGTH - 1);

    // Play current step if enabled
    if (_steps[_playBackStep].isEnabled())
    {
        // set current note being played
        _currentNotePlayed = _steps[_playBackStep].getNote();

        msgNoteOn.setType(midi::NoteOn);
        msgNoteOn.setDataByte1(_steps[_playBackStep].getNote());
        msgNoteOn.setDataByte2(127);
        _midiWorker->sendMIDIMessage(&msgNoteOn, _midiChannel);
    }

    // send note off message when legato is on
    if (isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);
    }
}

/*
* Stop current note being played
*/
void Sequencer::stopCurrentNotePlayed()
{
    MIDIMessage msgNoteOff;

    msgNoteOff.setType(midi::NoteOff);
    msgNoteOff.setDataByte1(_currentNotePlayed);
    _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);

    _stopCurrentPlayedNote = 0;
}

/*
* Send Note Off MIDI messages for all the steps within a sequence
*/
void Sequencer::stopAllNotes()
{
    // send note off messages before loading new sequence
    MIDIMessage msgNoteOff;

    //Send Note Off messages
    msgNoteOff.setType(midi::NoteOff);

    for (int i = 0; i < LENGTH; i++)
    {
        msgNoteOff.setDataByte1(_steps[i].getNote());
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);
    }
}

/*
* Display current sequence and total number of sequences available, general bpm and playback status
* syncManager: object that contains the global Bpm value
*/
void Sequencer::printDefault(SyncManager syncManager)
{
    _screenManager->printDefaultSequencer(_currentSequence, NUM_SEQUENCES, syncManager.getBpm(), _playBackOn);    
}

/*
* Display sequencer global configuration parameters
9 globalConfig: contains the global configuration of the system
*/
void Sequencer::printEditConfig(GlobalConfig globalConfig)
{
    _screenManager->printEditSequencerConfig(getPlayBackModeName(), getStepSizeName(), globalConfig.getSequencerMIDIChannel(), globalConfig.getSendClockWhilePlayback());
}

/*
* Display the step's information that is being played
*/
void Sequencer::updateDisplayedStep()
{

    switch (_playBackMode)
    {
    case FORWARD:

        if (_playBackStep == 0)
        {
            _screenManager->updateDisplayedPlaybackStep(_steps[LENGTH - 1], LENGTH, LENGTH);
        }

        else
        {
            _screenManager->updateDisplayedPlaybackStep(_steps[_playBackStep - 1], LENGTH, _playBackStep);
        }

        break;

    case BACKWARD:

        if (_playBackStep == LENGTH - 1)
        {
            _screenManager->updateDisplayedPlaybackStep(_steps[0], LENGTH, 1);
        }

        else
        {
            _screenManager->updateDisplayedPlaybackStep(_steps[_playBackStep + 1], LENGTH, _playBackStep + 2);
        }

        break;

    case RANDOM:

        _screenManager->updateDisplayedPlaybackStep(_steps[_playBackStep], LENGTH, _playBackStep + 1);

        break;
    }
}

/*
* Display the screen with the step information for edit: current step, total available steps, note of the step, legato and enabled values
*/
void Sequencer::printEditStepData()
{
    _screenManager->printEditStepData(_steps[0], 1, LENGTH);
}

/*
* Regarding current step, display the step information of the previos step within a sequence
*/
void Sequencer::printPreviousStep()
{
    if (_screenManager->getDisplayedStepNumber() - 1 > 0)
    {
        _screenManager->printEditStepData(_steps[_screenManager->getDisplayedStepNumber() - 2], _screenManager->getDisplayedStepNumber() - 1, LENGTH);
    }
}

/*
* Regarding current step, display the step information of the following step within a sequence
*/
void Sequencer::printNextStep()
{
    if (_screenManager->getDisplayedStepNumber() - 1 < LENGTH - 1)
    {
        _screenManager->printEditStepData(_steps[_screenManager->getDisplayedStepNumber()], _screenManager->getDisplayedStepNumber() + 1, LENGTH);
    }
}

/*
* When edit step information, move cursor to note value parameter
*/
void Sequencer::moveCursorToNote()
{
    _screenManager->moveCursorToStepNote();
}

/*
* When edit step information, move cursor to legato value parameter
*/
void Sequencer::moveCursorToLegato()
{
    _screenManager->moveCursorToStepLegato();
}

/*
* When edit step information, move cursor to enable parameter
*/
void Sequencer::moveCursorToEnabled()
{
    _screenManager->moveCursorToStepEnabled();
}

/*
* When edit sequencer configuration, move cursor to playback mode parameter
*/
void Sequencer::moveCursorToPlayBackMode()
{
    _screenManager->moveCursorToPlayBackMode();
}

/*
* When edit sequencer configuration, move cursor to send clock while playback parameter
*/
void Sequencer::moveCursorToSendClockWhilePlayback()
{
    _screenManager->moveCursorToSendClockWhilePlayback();
}

/*
* When edit sequencer configuration, move cursor to step size parameter
*/
void Sequencer::moveCursorToStepSize()
{
    _screenManager->moveCursorToStepSize();
}

/*
* When edit sequencer configuration, move cursor to sequencer MIDI channel parameter
*/
void Sequencer::moveCursorToMIDIChannel()
{
    _screenManager->moveCursorToSequencerMIDIChannel();
}

/*
* When edit sequencer configuration, update the value of the playback mode parameter
*/
void Sequencer::refreshDisplayedPlayBackMode(uint8_t playBackMode)
{
    _screenManager->refreshDisplayedPlayBackMode(getPlayBackModeName(playBackMode));
}

/*
* When edit sequencer configuration, update the value of the send clock while playback parameter
*/
void Sequencer::refreshDisplayedSendClockWhilePlayback(uint8_t sendClockWhilePlayback)
{
    _screenManager->refreshDisplayedSendClockWhilePlayback(sendClockWhilePlayback);
}

/*
* When edit sequencer configuration, update the value of the step size parameter
*/
void Sequencer::refreshDisplayedStepSizeValue(uint8_t stepSize)
{
    _screenManager->refreshDisplayedStepSizeValue(getStepSizeName(stepSize));
}

/*
* When edit sequencer configuration, update the value of the sequencer MIDI channel parameter
*/
void Sequencer::refreshDisplayedMIDIChannel(uint8_t midiChannel)
{
    _screenManager->refreshDisplayedSequencerMidiChannel(midiChannel);
}

/*
* Returns a char * with the playback mode to be displayed on the screen
*/
char *Sequencer::getPlayBackModeName()
{
    char buffer[10];

    switch (_playBackMode)
    {
    case FORWARD:
        getMessage(MSG_FORWARD, buffer);
        break;

    case BACKWARD:
        getMessage(MSG_BACKWARD, buffer);
        break;

    case RANDOM:
        getMessage(MSG_RANDOM, buffer);
        break;

    default:
        getMessage(MSG_NA, buffer);
    }

    return buffer;
}

/*
* Returns a char * with the step size to be displayed on the screen
*/
char *Sequencer::getStepSizeName()
{
    char buffer[5];

    switch (_stepSize)
    {
    case QUARTER:
        getMessage(MSG_QUARTER, buffer);
        break;

    case EIGHTH:
        getMessage(MSG_EIGHTH, buffer);
        break;

    case SIXTEENTH:
        getMessage(MSG_SIXTEENTH, buffer);
        break;

    case THIRTYSECOND:
        getMessage(MSG_THIRTYSECOND, buffer);
        break;

    default:
        getMessage(MSG_NA, buffer);
    }

    return buffer;
}

/*
* Returns a char * with the playback mode to be displayed on the screen
* playBackMode: the value to be returned
*/
char *Sequencer::getPlayBackModeName(uint8_t playBackMode)
{
    char buffer[10];

    switch (playBackMode)
    {
    case FORWARD:
        getMessage(MSG_FORWARD, buffer);
        break;

    case BACKWARD:
        getMessage(MSG_BACKWARD, buffer);
        break;

    case RANDOM:
        getMessage(MSG_RANDOM, buffer);
        break;

    default:
        getMessage(MSG_NA, buffer);
    }

    return buffer;
}

/*
* Returns a char * with the step size to be displayed on the screen
* stepSize: the value to be returned 
*/
char *Sequencer::getStepSizeName(uint8_t stepSize)
{
    char buffer[5];

    switch (stepSize)
    {
    case QUARTER:
        getMessage(MSG_QUARTER, buffer);
        break;

    case EIGHTH:
        getMessage(MSG_EIGHTH, buffer);
        break;

    case SIXTEENTH:
        getMessage(MSG_SIXTEENTH, buffer);
        break;

    case THIRTYSECOND:
        getMessage(MSG_THIRTYSECOND, buffer);
        break;

    default:
        getMessage(MSG_NA, buffer);
    }

    return buffer;
}

/*
* Load a string of chars from the PROGMEM
* msgIndex: text to Load
* buffer: array of chars where the text will be loaded
*/
void Sequencer::getMessage(uint8_t msgIndex, char *buffer)
{
    strcpy_P(buffer, (char *)pgm_read_word(&(sequencerMessages[msgIndex])));
}
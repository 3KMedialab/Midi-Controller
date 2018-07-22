#include "Sequencer.h"

Sequencer::Sequencer (uint8_t mode, uint8_t stepSize, MemoryManager * memoryManager, ScreenManager * screenManager)
{    
    _lastTimePlayBack = 0;
    _playBackOn = 0;    
    _stepSize = stepSize;
    _playBackMode = mode;
    _currentSequence = 1;

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

void Sequencer::setBpm(uint16_t * bpm)
{
    _bpm = bpm;
}

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

void Sequencer::setCurrentSequence (uint8_t numSequence)
{
    _currentSequence = numSequence;    
}

void Sequencer::setMidiWorker(MidiWorker * midiWorker)
{
    _midiWorker = midiWorker;
}

void Sequencer::setDisplayedStepNote(uint8_t note)
{
    _steps[_screenManager->getDisplayedStepNumber()-1].setNote(note);

    // refresh note value on screen
    _screenManager->refreshStepNoteValue(note);
}

void Sequencer::setDisplayedStepLegato(uint8_t legato)
{
    _steps[_screenManager->getDisplayedStepNumber()-1].setLegato(legato);

    // refresh legato value on screen
    _screenManager->refreshStepLegatoValue(legato);
}

void Sequencer::setDisplayedStepEnabled(uint8_t enabled)
{
    _steps[_screenManager->getDisplayedStepNumber()-1].setEnabled(enabled);

    // refresh enabled value on screen
    _screenManager->refreshStepEnabledValue(enabled);
}

void Sequencer::refreshDisplayedPlayBackMode(uint8_t playBackMode)
{
    _screenManager->refreshDisplayedPlayBackMode(getPlayBackModeName(playBackMode));
}

void Sequencer::refreshDisplayedStepSizeValue(uint8_t stepSize)
{
    _screenManager->refreshDisplayedStepSizeValue(getStepSizeName(stepSize));
}

void Sequencer::refreshDisplayedMIDIChannel(uint8_t midiChannel)
{
    _screenManager->refreshDisplayedSequencerMidiChannel(midiChannel);
}

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
    switch(stepSizeValue)
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

Step * Sequencer::getSequence()
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

void Sequencer::loadCurrentSequence()
{    
    _memoryManager->loadSequence(_currentSequence, _steps, LENGTH);
}

void Sequencer::saveCurrentSequence()
{
    _memoryManager->saveSequence(_currentSequence, _steps, LENGTH);
}

void Sequencer::startPlayBack()
{
    _playBackOn = 1;
}

void Sequencer::stopPlayBack()
{
    MIDIMessage msgNoteOff;

    // Mutes all sounding nots
    msgNoteOff.setType(midi::ControlChange);
    msgNoteOff.setDataByte1(midi::AllSoundOff);
    _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel); 
        
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
    _lastTimePlayBack = 0;
}

uint8_t Sequencer::playBackSequence(uint32_t currentTime)
{ 
    if (_playBackOn)
    {     
        if (currentTime - _lastTimePlayBack >= (MICROSECONDS_PER_MINUTE / *_bpm) / _stepSize)
        {
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

            _lastTimePlayBack = currentTime;

            return 1; // play next step is true                   
        }
    }

    return 0;
}

void Sequencer::playBackForward()
{
    MIDIMessage msgNoteOff;
    MIDIMessage msgNoteOn;
    uint8_t isStepLegato;

    // stop last played step
    msgNoteOff.setType(midi::NoteOff);
            
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
            
    if (!isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);       
    }

    // Play current step if enabled
    if (_steps[_playBackStep].isEnabled())
    {
        msgNoteOn.setType(midi::NoteOn);
        msgNoteOn.setDataByte1(_steps[_playBackStep].getNote());
        msgNoteOn.setDataByte2(127);
        _midiWorker->sendMIDIMessage(&msgNoteOn, _midiChannel);  
    }

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

void Sequencer::playBackBackward()
{
    MIDIMessage msgNoteOff;
    MIDIMessage msgNoteOn;
    uint8_t isStepLegato;

    // stop last played step
    msgNoteOff.setType(midi::NoteOff);
            
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
            
    if (!isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);       
    }

    // Play current step if enabled
    if (_steps[_playBackStep].isEnabled())
    {
        msgNoteOn.setType(midi::NoteOn);
        msgNoteOn.setDataByte1(_steps[_playBackStep].getNote());
        msgNoteOn.setDataByte2(127);
        _midiWorker->sendMIDIMessage(&msgNoteOn, _midiChannel);  
    }

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

void Sequencer::playBackRandom()
{
    MIDIMessage msgNoteOff;
    MIDIMessage msgNoteOn;
    uint8_t isStepLegato;

    // stop last played step
    msgNoteOff.setType(midi::NoteOff);
    msgNoteOff.setDataByte1(_steps[_playBackStep].getNote());
    
    isStepLegato = _steps[_playBackStep].isLegato();
               
    if (!isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);       
    }

    // Calculate next step
    _playBackStep = random(0, LENGTH - 1);

    // Play current step if enabled
    if (_steps[_playBackStep].isEnabled())
    {
        msgNoteOn.setType(midi::NoteOn);
        msgNoteOn.setDataByte1(_steps[_playBackStep].getNote());
        msgNoteOn.setDataByte2(127);
        _midiWorker->sendMIDIMessage(&msgNoteOn, _midiChannel);  
    }

    if (isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);       
    }
}

void Sequencer::printDefault()
{
    _screenManager->printDefaultSequencer(_currentSequence, NUM_SEQUENCES, *_bpm);
    _screenManager->updateDisplayedPlaybackStep(_steps[_playBackStep], LENGTH, _playBackStep + 1);
}

void Sequencer::printEditConfig(GlobalConfig globalConfig)
{
    _screenManager->printEditSequencerConfig(getPlayBackModeName(), getStepSizeName(), globalConfig.getSequencerMIDIChannel());
}

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

void Sequencer::printEditStepData()
{
    _screenManager->printEditStepData(_steps[0], 1, LENGTH);
}

void Sequencer::printPreviousStep()
{
    if (_screenManager->getDisplayedStepNumber() - 1 > 0)
    {
        _screenManager->printEditStepData(_steps[_screenManager->getDisplayedStepNumber() - 2], _screenManager->getDisplayedStepNumber() - 1, LENGTH);     
    }
}

void Sequencer::printNextStep()
{
    if (_screenManager->getDisplayedStepNumber() - 1 < LENGTH - 1)
    {
        _screenManager->printEditStepData(_steps[_screenManager->getDisplayedStepNumber()], _screenManager->getDisplayedStepNumber() + 1, LENGTH);       
    }
}

void Sequencer::moveCursorToNote()
{
    _screenManager->moveCursorToStepNote();
}

void Sequencer::moveCursorToLegato()
{
    _screenManager->moveCursorToStepLegato();
}

void Sequencer::moveCursorToEnabled()
{
    _screenManager->moveCursorToStepEnabled();
}

void Sequencer::moveCursorToPlayBackMode()
{
    _screenManager->moveCursorToPlayBackMode();
}

void Sequencer::moveCursorToStepSize()
{
    _screenManager->moveCursorToStepSize();
}

void Sequencer::moveCursorToMIDIChannel()
{
    _screenManager->moveCursorToSequencerMIDIChannel();
}

String Sequencer::getPlayBackModeName()
{     
    switch(_playBackMode)
    {    
       case FORWARD:    return F("Forward");    
       case BACKWARD:   return F("Backward");
       case RANDOM:     return F("Random"); 
       default:         return F("N/A");        
    }
}

String Sequencer::getStepSizeName()
{
    switch(_stepSize)
    {    
       case QUARTER:        return F("1/4");    
       case EIGHTH:         return F("1/8");
       case SIXTEENTH:      return F("1/16");
       case THIRTYSECOND:   return F("1/32");
       default:             return F("N/A");        
    }
}

String Sequencer::getPlayBackModeName(uint8_t playBackMode)
{     
    switch(playBackMode)
    {    
       case FORWARD:    return F("Forward");    
       case BACKWARD:   return F("Backward");
       case RANDOM:     return F("Random"); 
       default:         return F("N/A");        
    }
}

String Sequencer::getStepSizeName(uint8_t stepSize)
{
    switch(stepSize)
    {    
       case QUARTER:        return F("1/4");    
       case EIGHTH:         return F("1/8");
       case SIXTEENTH:      return F("1/16");
       case THIRTYSECOND:   return F("1/32");
       default:             return F("N/A");        
    }
}
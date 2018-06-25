#include "Sequencer.h"

Sequencer::Sequencer (MidiWorker * midiWorker, Sequence * sequence, uint8_t mode, uint8_t stepSize)
{
    _midiWorker = midiWorker;
    _sequence = sequence;
    _lastTimePlayBack = 0;
    _playBackOn = 0;    
    _stepSize = stepSize;
    _playBackMode = mode;

    switch (_playBackMode)
    {
        case FORWARD:
        case RANDOM:
            _playBackStep = 0;
        break;

        case BACKWARD:
            _playBackStep = _sequence->getLength() - 1;
        break;
    }
}

void Sequencer::setSequence(Sequence * sequence)
{
    _sequence = sequence;
}

void Sequencer::setBpm(uint8_t bpm)
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
                _playBackStep = _sequence->getLength() - 1;
            break;
        }
    }
}

void Sequencer::setStepSize (uint8_t size)
{
    _stepSize = size;
}

Sequence * Sequencer::getSequence()
{
    return _sequence;
}

int8_t Sequencer::getPlayBackStep()
{
    return _playBackStep;
}

uint8_t Sequencer::isPlayBackOn()
{
    return _playBackOn; 
}

uint8_t Sequencer::getMIDIChannel()
{
    return _midiChannel;
}

void Sequencer::startPlayBack()
{
    _playBackOn = 1;
}

void Sequencer::stopPlayBack()
{
    _playBackOn = 0;
    _lastTimePlayBack = 0;

    switch (_playBackMode)
    {
        case FORWARD:
        case RANDOM:
            _playBackStep = 0;
        break;

        case BACKWARD:
            _playBackStep = _sequence->getLength() - 1;
        break;
    }
}

void Sequencer::playBackSequence(uint32_t currentTime)
{ 
    if (_playBackOn)
    {     
        if (currentTime - _lastTimePlayBack >= (MICROSECONDS_PER_MINUTE / _bpm) / _stepSize)
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
        }
    }
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
        msgNoteOff.setDataByte1(_sequence->getStep(_sequence->getLength() - 1).getNote());
        isStepLegato = _sequence->getStep(_sequence->getLength() - 1).isLegato();
    }

    else
    {
        msgNoteOff.setDataByte1(_sequence->getStep(_playBackStep - 1).getNote());
        isStepLegato = _sequence->getStep(_playBackStep - 1).isLegato();
    }            
            
    if (!isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);       
    }

    // Play current step if enabled
    if (_sequence->getStep(_playBackStep).isEnabled())
    {
        msgNoteOn.setType(midi::NoteOn);
        msgNoteOn.setDataByte1(_sequence->getStep(_playBackStep).getNote());
        msgNoteOn.setDataByte2(127);
        _midiWorker->sendMIDIMessage(&msgNoteOn, _midiChannel);  
    }

    if (isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);       
    }

    // Move sequence to next step
    _playBackStep++;

    if (_playBackStep >= _sequence->getLength())
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
            
    if (_playBackStep == _sequence->getLength() - 1)
    {
        msgNoteOff.setDataByte1(_sequence->getStep(0).getNote());
        isStepLegato = _sequence->getStep(0).isLegato();
    }

    else
    {
        msgNoteOff.setDataByte1(_sequence->getStep(_playBackStep + 1).getNote());
        isStepLegato = _sequence->getStep(_playBackStep + 1).isLegato();
    }            
            
    if (!isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);       
    }

    // Play current step if enabled
    if (_sequence->getStep(_playBackStep).isEnabled())
    {
        msgNoteOn.setType(midi::NoteOn);
        msgNoteOn.setDataByte1(_sequence->getStep(_playBackStep).getNote());
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
        _playBackStep = _sequence->getLength() - 1;
    }        
}

void Sequencer::playBackRandom()
{
    MIDIMessage msgNoteOff;
    MIDIMessage msgNoteOn;
    uint8_t isStepLegato;

    // stop last played step
    msgNoteOff.setType(midi::NoteOff);
    msgNoteOff.setDataByte1(_sequence->getStep(_playBackStep).getNote());
    
    isStepLegato = _sequence->getStep(_playBackStep).isLegato();
               
    if (!isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);       
    }

    // Calculate next step
    _playBackStep = random(0, _sequence->getLength() - 1);

    // Play current step if enabled
    if (_sequence->getStep(_playBackStep).isEnabled())
    {
        msgNoteOn.setType(midi::NoteOn);
        msgNoteOn.setDataByte1(_sequence->getStep(_playBackStep).getNote());
        msgNoteOn.setDataByte2(127);
        _midiWorker->sendMIDIMessage(&msgNoteOn, _midiChannel);  
    }

    if (isStepLegato)
    {
        _midiWorker->sendMIDIMessage(&msgNoteOff, _midiChannel);       
    }
}
   
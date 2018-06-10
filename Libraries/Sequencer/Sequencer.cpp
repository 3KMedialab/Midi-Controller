#include "Sequencer.h"

Sequencer::Sequencer (Sequence * sequence)
{
    _sequence = sequence;
}

void Sequencer::setSequence(Sequence * sequence)
{
    _sequence = sequence;
}

void Sequencer::setPlayBackOn(uint8_t playBackState)
{
    _playBackOn = playBackState;
}

void Sequencer::setStepDelay(uint32_t delay)
{
    _stepDelay = delay;
}

void Sequencer::setLastTimePlayBack(uint32_t time)
{
    _lastTimePlayBack = time;
}

void Sequencer::setPlayBackStep(uint8_t currentStep)
{
    _playBackStep = currentStep;
}

void Sequencer::setMIDIChannel(uint8_t channel)
{
    _midiChannel = channel;
}

Sequence * Sequencer::getSequence()
{
    return _sequence;
}

uint32_t Sequencer::getStepDelay()
{
    return _stepDelay;
}

uint32_t Sequencer::getLastTimePlayBack()
{
    return _lastTimePlayBack;
}

uint8_t Sequencer::getPlayBackStep()
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

void Sequencer::updatePlaybackStatus()
{
    _playBackOn = !_playBackOn;
}

void Sequencer::playBackSequence(uint32_t currentTime)
{
    if (_playBackOn)
    {
        if (currentTime - _lastTimePlayBack >= _stepDelay)
        {
            _lastTimePlayBack = currentTime;  

            //Stop playback current step
           // _mMidi.sendNoteOff(_sequence->getStep(_playBackStep).getNote(), 127, _midiChannel);

            // Move pointer to next step
            _playBackStep++;

            if (_playBackStep >= _sequence->getLength() - 1)
            {
                _playBackStep = 0;
            }                            
 
            // Play next step            
            //_mMidi.sendNoteOn(_sequence->getStep(_playBackStep).getNote(), 127, _midiChannel);
                        
        }
    }
}
   
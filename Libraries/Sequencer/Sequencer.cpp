#include "Sequencer.h"

Sequencer::Sequencer (MidiWorker * midiWorker, Sequence * sequence)
{
    _midiWorker = midiWorker;
    _sequence = sequence;
}

void Sequencer::setSequence(Sequence * sequence)
{
    _sequence = sequence;
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

void Sequencer::startPlayBack()
{
    _playBackOn = 1;
}

void Sequencer::stopPlayBack()
{
    _playBackOn = 0;
    _playBackStep = 0;
}

void Sequencer::playBackSequence(uint32_t currentTime)
{
    if (_playBackOn)
    {     
        if (currentTime - _lastTimePlayBack >= _stepDelay)
        {
            // stop last played step
            _message.setType(midi::NoteOff);
            
            if (_playBackStep == 0)
            {
               _message.setDataByte1(_sequence->getStep(_sequence->getLength() - 1).getNote());
            }

            else
            {
                _message.setDataByte1(_sequence->getStep(_playBackStep - 1).getNote());
            }            
            
            _midiWorker->sendMIDIMessage(&_message, _midiChannel);       

            // Play current step            
            _message.setType(midi::NoteOn);
            _message.setDataByte1(_sequence->getStep(_playBackStep).getNote());
            _message.setDataByte2(127);
            _midiWorker->sendMIDIMessage(&_message, _midiChannel);  

            // Move pointer to next step
            _playBackStep++;

            if (_playBackStep >= _sequence->getLength() - 1)
            {
                _playBackStep = 0;
            }

            _lastTimePlayBack = currentTime;                       
        }
    }
}
   
#include "Sequencer.h"

Sequencer::Sequencer (MidiWorker * midiWorker, Sequence * sequence)
{
    _midiWorker = midiWorker;
    _sequence = sequence;
    _lastTimePlayBack = 0;
    _playBackOn = 0;
    _playBackStep = 0;
}

void Sequencer::setSequence(Sequence * sequence)
{
    _sequence = sequence;
}

void Sequencer::setBpm(uint8_t bpm)
{
    _bpm = bpm;
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
    _lastTimePlayBack = 0;
}

void Sequencer::playBackSequence(uint32_t currentTime)
{
    if (_playBackOn)
    {     
        if (currentTime - _lastTimePlayBack >= (MICROSECONDS_PER_MINUTE / _bpm))
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

            // Move sequence to next step
            _playBackStep++;

            if (_playBackStep >= _sequence->getLength())
            {
                _playBackStep = 0;
            }

            _lastTimePlayBack = currentTime;                       
        }
    }
}
   
#include "Sequence.h"

Sequence::Sequence()
{
    _length = MAX_LENGTH;

    for (int i=0;i<MAX_LENGTH;i++)
    {
        _steps[i].setNote(60 + i);  
    }
}

void Sequence::setStep (Step step, uint8_t position)
{
    _steps[position] = step;
}

void Sequence::setLength(uint8_t length)
{
    _length = length;
}

Step * Sequence::getSteps()
{
    return _steps;
}

Step Sequence::getStep(uint8_t position)
{
    return _steps[position];
}

uint8_t Sequence::getLength()
{
    return _length;
}
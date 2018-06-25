#include "Step.h"

uint8_t Step::getNote()
{
    return _note;
}

uint8_t Step::isEnabled()
{
    return _enabled;
}

uint8_t Step::isLegato()
{
    return _legato;
}

void Step::setNote(uint8_t note)
{
    _note = note;
}

void Step::setEnabled(uint8_t enabled)
{
    _enabled = enabled;
}

void Step::setLegato(uint8_t legato)
{
    _legato = legato;
}  


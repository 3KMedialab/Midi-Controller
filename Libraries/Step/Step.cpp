#include "Step.h"

Step::Step()
{
    
}

Step::Step (uint8_t note, uint8_t enabled, uint8_t legato)
{
	_note = note;
    _enabled = enabled;
    _legato = legato;
}

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

static uint8_t Step::getSize()
{
	return (sizeof(uint8_t) * 3);
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



/*
 * Step.h
 *
 * This class represents a Step within a sequence. Each step is composed of the following attributes:
 * Note: is the MIDI note that will be played
 * Enabled: wether the note should be played or not.
 * legato: wether the note should be played legato with the foloowing step
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

#include "Step.h"

Step::Step(){}

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
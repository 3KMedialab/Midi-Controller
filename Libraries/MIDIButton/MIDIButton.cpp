/*
  MIDIButton.cpp - Class that defines a MIDI Button.
  Created by: Daniel Segura, August 21, 2017
*/

#include <MIDIButton.h>

MIDIButton::MIDIButton(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime, MIDIMessage * onPressedMessage, MIDIMessage * onReleasedMessage) : Button (pin, puEnable, invert, dbTime)
{
    _onPressedMessage = onPressedMessage;
    _onReleasedMessage = onReleasedMessage;
}

MIDIMessage MIDIButton::getOnPressedMessage()
{
    return *_onPressedMessage;
}

MIDIMessage MIDIButton::getOnReleasedMessage()
{
    return *_onReleasedMessage;
}


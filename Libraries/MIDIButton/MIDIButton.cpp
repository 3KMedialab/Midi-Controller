/*
  MIDIButton.cpp - Class that defines a MIDI Button.
  Created by: Daniel Segura, August 21, 2017
*/

#include <MIDIButton.h>

MIDIButton::MIDIButton(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime, MIDIMessage * messages, MIDIMessage * shiftMessages) : Button (pin, puEnable, invert, dbTime)
{
    _messages = messages;
    _shiftMessages = shiftMessages;
}

MIDIMessage MIDIButton::getMessage(uint8_t msgId)
{
    return _messages[msgId];
}

MIDIMessage MIDIButton::getShiftMessage(uint8_t msgId)
{
    return _shiftMessages[msgId];
}


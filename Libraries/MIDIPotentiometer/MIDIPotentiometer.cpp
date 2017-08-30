/*
  MIDIPotentiometer.cpp - Class that defines a MIDI Potentiometer.
  Created by: Daniel Segura, August 24, 2017
*/

#include <MIDIPotentiometer.h>

MIDIPotentiometer::MIDIPotentiometer(uint8_t pin, MIDIMessage * message) : Potentiometer (pin)
{
    _message = message;
}

MIDIMessage MIDIPotentiometer::getMessage()
{
    return *_message;
}
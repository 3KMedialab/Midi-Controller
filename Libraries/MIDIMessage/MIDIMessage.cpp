/*
  MIDIMessage.cpp - Class that defines a MIDI Message.
  Created by: Daniel Segura, August 121, 2017
*/

#include "MIDIMessage.h"

/**
  Constructor for MIDI Message.

  @pin 
*/

MIDIMessage::MIDIMessage(uint8_t type, uint8_t dataByte1, uint8_t dataByte2, uint8_t channel)
{  
  _type = type;
  _dataByte1 = dataByte1;
  _dataByte2 = dataByte2;
  _channel = channel;
}

uint8_t MIDIMessage::getType()
{
  return _type;
}

uint8_t MIDIMessage::getDataByte1()
{
  return _dataByte1;
}

uint8_t MIDIMessage::getDataByte2()
{
  return _dataByte2;
}         

uint8_t MIDIMessage::getChannel()
{
  return _channel;
}
/* 
    MIDIMessage.h - Class for define a MIDI Message
    Created by: Daniel Segura, August 21, 2017
*/

#ifndef MIDIMessage_h
#define MIDIMessage_h

#include "Arduino.h"

class MIDIMessage
{
  public:
    MIDIMessage(); // constructor for MIDI Messages
    MIDIMessage(uint8_t type, uint8_t dataByte1, uint8_t dataByte2, uint8_t channel); // constructor for MIDI Messages
    
    uint8_t getType();
    uint8_t getDataByte1();
    uint8_t getDataByte2();
    uint8_t getChannel();

    void setType(uint8_t type);
    void setDataByte1(uint8_t dataByte1);
    void setDataByte2(uint8_t dataByte2);
    void setChannel(uint8_t channel);

  private:
    uint8_t _type;      // MIDI message type
    uint8_t _dataByte1; // data byte 1
    uint8_t _dataByte2; // data byte 2
    uint8_t _channel;   // MIDI channel
};
#endif
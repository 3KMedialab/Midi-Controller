/*
 * MIDIMessage.h
 *
 * Class that represents a MIDI Message. These objects will be assigned to the different
 * MIDI Components that the controller can manage.
 *
 * Copyright 2017 3K MEDIALAB
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
#ifndef MIDIMessage_h
#define MIDIMessage_h

#include "Arduino.h"

class MIDIMessage
{
  public:
    MIDIMessage(); 
    MIDIMessage(uint8_t type, uint8_t dataByte1, uint8_t dataByte2); 
    
    uint8_t getType();
    uint8_t getDataByte1();
    uint8_t getDataByte2();

    void setType(uint8_t type);
    void setDataByte1(uint8_t dataByte1);
    void setDataByte2(uint8_t dataByte2);  

  private:
    uint8_t _type;      // MIDI message type
    uint8_t _dataByte1; // data byte 1
    uint8_t _dataByte2; // data byte 2 
};
#endif
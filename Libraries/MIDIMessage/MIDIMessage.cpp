/*
 * MIDIMessage.cpp
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
#include "MIDIMessage.h"

/*
* Constructor
* type: the type of the Message
* dataByte1: first byte of data of the message.
* dataByte2: second byte of data of the message.
* channel: the channel over the message will be send.
*/
MIDIMessage::MIDIMessage(uint8_t type, uint8_t dataByte1, uint8_t dataByte2)
{  
  _type = type;
  _dataByte1 = dataByte1;
  _dataByte2 = dataByte2; 
}

/*
* Constructor
*/
MIDIMessage::MIDIMessage()
{  
  _type = 0;
  _dataByte1 = 0;
  _dataByte2 = 0; 
}

/*
* Getters methods
*/
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

/*
* Setter methods.
*/
void MIDIMessage::setType(uint8_t type)
{
  _type = type;
}

void MIDIMessage::setDataByte1(uint8_t dataByte1)
{
  _dataByte1 = dataByte1;
}

void MIDIMessage::setDataByte2(uint8_t dataByte2)
{
  _dataByte2 = dataByte2;
}
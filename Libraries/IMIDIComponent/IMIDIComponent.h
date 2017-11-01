/*
 * IMIDIComponent.h
 *
 * Interface that defines a MIDI component
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

#ifndef IMIDIComponent_h
#define IMIDIComponent_h

#include "Arduino.h"
#include "MIDIMessage.h"

class IMIDIComponent
{
  public:
    IMIDIComponent();
    virtual MIDIMessage * getMessageToSend() = 0;
    virtual uint8_t getNumMessages() = 0;
    virtual MIDIMessage * getMessages() = 0;
    virtual uint8_t getDataSize() = 0;	   
};
#endif
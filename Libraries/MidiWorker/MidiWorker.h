/*
 * MidiWorker.h
 *
 * Class that manage all the functionality related to sending MIDI messages 
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
#ifndef MidiWorker_h
#define MidiWorker_h

#include <MIDI.h>
#include <MIDIMessage.h>

typedef midi::MidiInterface<HardwareSerial> MidiInterface;

class MidiWorker
{
  public:   
    MidiWorker();   
    MidiWorker(MidiInterface& inInterface);   
    void begin();
    void sendMIDIMessage(MIDIMessage * message, uint8_t channel);
    void sendMIDIStartClock();
    void sendMIDIClock();
    void sendMIDIStopClock();

  private:

    MidiInterface& _mMidi; // the MIDI object interface
   
};
#endif
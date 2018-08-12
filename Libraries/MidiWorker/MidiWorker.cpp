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
#include "MidiWorker.h"

MidiWorker::MidiWorker()
{}

MidiWorker::MidiWorker(MidiInterface& inInterface)
: _mMidi(inInterface)
{}

/*
* Initializes the MIDI inInterface
*/
void MidiWorker::begin()
{
    _mMidi.begin();
}

/*
* Send a MIDI message regarding its type
* message: the MIDI message to be sent.
* channel: MIDI channel where to send the message
*/
void MidiWorker::sendMIDIMessage(MIDIMessage * message, uint8_t channel)
{ 
    switch(message->getType())
    {
        case midi::ControlChange:
           _mMidi.sendControlChange(message->getDataByte1(), message->getDataByte2(), channel);
        break;

        case midi::ProgramChange:
           _mMidi.sendProgramChange(message->getDataByte1(), channel);
        break;

        case midi::NoteOn:
           _mMidi.sendNoteOn(message->getDataByte1(), message->getDataByte2(), channel);
        break;

        case midi::NoteOff:
           _mMidi.sendNoteOff(message->getDataByte1(), message->getDataByte2(), channel);
        break;

        case midi::InvalidType:        
        break;
    }
}

/*
* Send start MIDI clock signal
*/
void MidiWorker::sendMIDIStartClock()
{  
    _mMidi.sendRealTime(midi::Start);  
}

/*
* Send MIDI clock signal
*/
void MidiWorker::sendMIDIClock()
{
    _mMidi.sendRealTime(midi::Clock);
}

/*
* Send stop MIDI clock signal
*/
void MidiWorker::sendMIDIStopClock()
{
    _mMidi.sendRealTime(midi::Stop);
}
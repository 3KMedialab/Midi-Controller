#include "MidiWorker.h"

MidiWorker::MidiWorker()
{}

MidiWorker::MidiWorker(MidiInterface& inInterface)
: _mMidi(inInterface)
{}

void MidiWorker::begin()
{
    _mMidi.begin();
}

/*
* Send a MIDI message regarding its type
* message: the MIDI message to be sent.
*/
/*void MidiWorker::sendMIDIMessage(MIDIMessage * message, uint8_t channel)
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
}*/

/*
* Send a MIDI message regarding its type
* message: the MIDI message to be sent.
*/
void MidiWorker::sendMIDIMessage(MIDIMessage * message, uint8_t channel)
{ 
    Serial.println(F("MESSAGE"));
    Serial.println(message->getType(),DEC);
    Serial.println(message->getDataByte1(),DEC);
    Serial.println(message->getDataByte2(),DEC);
    Serial.println(channel,DEC);  
   
}

/*
* Send start MIDI clock signal
*/
/*void MidiWorker::sendMIDIStartClock()
{  
    _mMidi.sendRealTime(midi::Start);  
}*/

/*
* Send MIDI clock signal
*/
/*void MidiWorker::sendMIDIClock()
{
    _mMidi.sendRealTime(midi::Clock);
}*/

/*
* Send stop MIDI clock signal
*/
/*void MidiWorker::sendMIDIStopClock()
{
    _mMidi.sendRealTime(midi::Stop);
}*/

void MidiWorker::sendMIDIStartClock()
{  
    Serial.println(F("MESSAGE"));
    Serial.println(F("START"));  
}

/*
* Send MIDI clock signal
*/
void MidiWorker::sendMIDIClock()
{
    /*Serial.println(F("MESSAGE"));
    Serial.println(F("CLOCK"));  */
}

/*
* Send stop MIDI clock signal
*/
void MidiWorker::sendMIDIStopClock()
{
    Serial.println(F("MESSAGE"));
    Serial.println(F("STOP"));  
}
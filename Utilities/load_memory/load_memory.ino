/*
 * MIDI messages loader
 *
 * Load the EEPROM with the default MIDI messages the controller will use
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

#include <Pitches.h>
#include <MIDI.h>
#include <MIDIButton.h>
#include <MIDIPotentiometer.h>
#include <EEPROM.h>  

#define MESSAGES 15

// PAGE 1
MIDIMessage p1b1m1(midi::NoteOn, NOTE_C3, 127, 1);
MIDIMessage p1b1m2(midi::NoteOff, NOTE_C3, 127, 1);
MIDIMessage p1b2m1(midi::NoteOn, NOTE_D3, 127, 1);
MIDIMessage p1b2m2(midi::NoteOff, NOTE_D3, 127, 1);
MIDIMessage p1p1m(midi::ControlChange, midi::BreathController, 0, 1);

// PAGE 2
MIDIMessage p2b1m1(midi::NoteOn, NOTE_C4, 127, 1);
MIDIMessage p2b1m2(midi::NoteOff, NOTE_C4, 127, 1);
MIDIMessage p2b2m1(midi::NoteOn, NOTE_D4, 127, 1);
MIDIMessage p2b2m2(midi::NoteOff, NOTE_D4, 127, 1);
MIDIMessage p2p1m(midi::ControlChange, midi::ChannelVolume, 0, 1);

// PAGE 3
MIDIMessage p3b1m1(midi::NoteOn, NOTE_C5, 127, 1);
MIDIMessage p3b1m2(midi::NoteOff, NOTE_C5, 127, 1);
MIDIMessage p3b2m1(midi::NoteOn, NOTE_D5, 127, 1);
MIDIMessage p3b2m2(midi::NoteOff, NOTE_D5, 127, 1);
MIDIMessage p3p1m(midi::ControlChange, midi::Pan, 0, 1);

uint16_t address = 0;
 
 void setup(void)
 {
   Serial.begin(9600);

   // PAGE 1
   saveMIDIMessage(&address,&p1b1m1);
   saveMIDIMessage(&address,&p1b1m2); 
   saveMIDIMessage(&address,&p1b2m1);
   saveMIDIMessage(&address,&p1b2m2); 
   saveMIDIMessage(&address,&p1p1m);

   // PAGE 2
   saveMIDIMessage(&address,&p2b1m1);
   saveMIDIMessage(&address,&p2b1m2); 
   saveMIDIMessage(&address,&p2b2m1);
   saveMIDIMessage(&address,&p2b2m2); 
   saveMIDIMessage(&address,&p2p1m);

   // PAGE 3
   saveMIDIMessage(&address,&p3b1m1);
   saveMIDIMessage(&address,&p3b1m2);
   saveMIDIMessage(&address,&p3b2m1);
   saveMIDIMessage(&address,&p3b2m2);
   saveMIDIMessage(&address,&p3p1m);
  
   address = 0;

   // Print the stored messages
   for (int i=0;i<MESSAGES*4;i++)
   {   
    if (i%4==0)
    {
      Serial.print("MESSAGE: ");  
      Serial.println(i/4,DEC); 
    }
    
    Serial.println(EEPROM.read(address),DEC);
    address++;
    
   }
 }
 
 void loop(void)
 {
    
 }

void saveMIDIMessage(uint16_t * address, MIDIMessage * message)
{
    EEPROM.update((*address),message->getType());
    (*address) += sizeof(uint8_t);
    EEPROM.update((*address),message->getDataByte1());
    (*address) += sizeof(uint8_t);
    EEPROM.update((*address),message->getDataByte2());
    (*address) += sizeof(uint8_t);
    EEPROM.update((*address),message->getChannel());
    (*address) += sizeof(uint8_t);
}



/*
   MIDI messages loader

   Load the EEPROM with the default MIDI messages the controller will use

   Copyright 2017 3K MEDIALAB

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include <Pitches.h>
#include <MIDI.h>
#include <MIDIButton.h>
#include <MIDIPotentiometer.h>
#include <EEPROM.h>
#include <GlobalConfig.h>
#include <MIDIUtils.h>

/**************************************************************/
/* CONFIGURATION FOR 9 MIDI BUTTONS AND 3 MIDI POTENTIOMETERS */
/* NUM PAGES: 9                                              */
/* NUM MESSAGES: 252                                          */
/**************************************************************/
#define MEMORY_SIZE 1024
#define MESSAGES 252

#define MIDI_BUTTONS_NUM 9
#define MIDI_BUTTONS_SIZE 4 * 2

#define MIDI_POTS_NUM 3
#define MIDI_POTS_SIZE 4

// create the global configuration object
GlobalConfig _config = GlobalConfig(1, MIDIUtils::Aeolian, MIDIUtils::C);

// PAGE DATA
MIDIMessage b1m1(midi::NoteOn, NOTE_C3, 127, 1);
MIDIMessage b1m2(midi::NoteOff, NOTE_C3, 127, 1);
MIDIMessage b2m1(midi::NoteOn, NOTE_Cb3, 127, 1);
MIDIMessage b2m2(midi::NoteOff, NOTE_Cb3, 127, 1);
MIDIMessage b3m1(midi::NoteOn, NOTE_D3, 127, 1);
MIDIMessage b3m2(midi::NoteOff, NOTE_D3, 127, 1);
MIDIMessage b4m1(midi::NoteOn, NOTE_Db3, 127, 1);
MIDIMessage b4m2(midi::NoteOff, NOTE_Db3, 127, 1);
MIDIMessage b5m1(midi::NoteOn, NOTE_E3, 127, 1);
MIDIMessage b5m2(midi::NoteOff, NOTE_E3, 127, 1);
MIDIMessage b6m1(midi::NoteOn, NOTE_F3, 127, 1);
MIDIMessage b6m2(midi::NoteOff, NOTE_F3, 127, 1);
MIDIMessage b7m1(midi::NoteOn, NOTE_Fb3, 127, 1);
MIDIMessage b7m2(midi::NoteOff, NOTE_Fb3, 127, 1);
MIDIMessage b8m1(midi::NoteOn, NOTE_G3, 127, 1);
MIDIMessage b8m2(midi::NoteOff, NOTE_G3, 127, 1);
MIDIMessage b9m1(midi::NoteOn, NOTE_Gb3, 127, 1);
MIDIMessage b9m2(midi::NoteOff, NOTE_Gb3, 127, 1);
MIDIMessage p1m(midi::ControlChange, midi::BreathController, 0, 1);
MIDIMessage p2m(midi::ControlChange, midi::BreathController, 0, 2);
MIDIMessage p3m(midi::ControlChange, midi::BreathController, 0, 3);

uint16_t address = 0;
uint8_t pageSize = (MIDI_BUTTONS_NUM * MIDI_BUTTONS_SIZE) + (MIDI_POTS_NUM * MIDI_POTS_SIZE);
uint8_t maxNumPages = (MEMORY_SIZE - GlobalConfig::getSize())  / pageSize;


void setup(void)
{
  Serial.begin(9600);

  // GLOBAL CONFIG DATA
  EEPROM.update(address, _config.getMIDIChannel());
  address += sizeof(uint8_t);
  EEPROM.update(address, _config.getMode());
  address += sizeof(uint8_t);
  EEPROM.update(address, _config.getRootNote());
  address += sizeof(uint8_t);

  for (int i = 0; i < maxNumPages; i++)
  {
    // PAGE DATA
    saveMIDIMessage(&address, b1m1);
    saveMIDIMessage(&address, b1m2);

    saveMIDIMessage(&address, b2m1);
    saveMIDIMessage(&address, b2m2);

    saveMIDIMessage(&address, b3m1);
    saveMIDIMessage(&address, b3m2);

    saveMIDIMessage(&address, b4m1);
    saveMIDIMessage(&address, b4m2);

    saveMIDIMessage(&address, b5m1);
    saveMIDIMessage(&address, b5m2);

    saveMIDIMessage(&address, b6m1);
    saveMIDIMessage(&address, b6m2);

    saveMIDIMessage(&address, b7m1);
    saveMIDIMessage(&address, b7m2);

    saveMIDIMessage(&address, b8m1);
    saveMIDIMessage(&address, b8m2);

    saveMIDIMessage(&address, b9m1);
    saveMIDIMessage(&address, b9m2);

    saveMIDIMessage(&address, p1m);
    saveMIDIMessage(&address, p2m);
    saveMIDIMessage(&address, p3m);
  }

  address = 0;

  // Print the global configuration
  Serial.println("MIDI CHANNEL:");
  Serial.println(EEPROM.read(address), DEC);
  address++;

  Serial.println("MODE:");
  Serial.println(EEPROM.read(address), DEC);
  address++;

  Serial.println("ROOT NOTE");
  Serial.println(EEPROM.read(address), DEC);
  address++;

  // Print the stored messages
  for (int i = 0; i < (((MIDI_BUTTONS_NUM * 2) + (MIDI_BUTTONS_NUM))*maxNumPages) * 4; i++)
  {
    if (i % 4 == 0)
    {
      Serial.print("MESSAGE: ");
      Serial.println(i / 4, DEC);
    }

    Serial.println(EEPROM.read(address), DEC);
    address++;

  }
}

void loop(void)
{

}

void saveMIDIMessage(uint16_t * address, MIDIMessage message)
{
  EEPROM.update((*address), message.getType());
  (*address) += sizeof(uint8_t);
  EEPROM.update((*address), message.getDataByte1());
  (*address) += sizeof(uint8_t);
  EEPROM.update((*address), message.getDataByte2());
  (*address) += sizeof(uint8_t);
  EEPROM.update((*address), message.getChannel());
  (*address) += sizeof(uint8_t);
}



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
#include <Step.h>

/****************************************************************/
/* CONFIGURATION FOR: 											*/
/* 10 PAGES WITH 9 MIDI BUTTONS AND 3 MIDI POTENTIOMETERS 		*/
/* 10 SEQUENCES WITH 8 STEPS							 		*/
/****************************************************************/
#define MEMORY_SIZE 1024

#define NUM_PAGES 10
#define NUM_SEQUENCES 10

#define MIDI_BUTTONS_NUM 9
#define MIDI_BUTTONS_SIZE 3

#define MIDI_POTS_NUM 3
#define MIDI_POTS_SIZE 3

#define SEQUENCE_LENGTH 8
#define STEP_SIZE 3

// create the global configuration object
GlobalConfig _config = GlobalConfig(1, 2, MIDIUtils::Aeolian, MIDIUtils::C, 1);

// PAGE DATA
MIDIMessage b1m(midi::NoteOn, NOTE_C3, 127);
MIDIMessage b2m(midi::NoteOn, NOTE_Cb3, 127);
MIDIMessage b3m(midi::NoteOn, NOTE_D3, 127);
MIDIMessage b4m(midi::NoteOn, NOTE_Db3, 127);
MIDIMessage b5m(midi::NoteOn, NOTE_E3, 127);
MIDIMessage b6m(midi::NoteOn, NOTE_F3, 127);
MIDIMessage b7m(midi::NoteOn, NOTE_Fb3, 127);
MIDIMessage b8m(midi::NoteOn, NOTE_G3, 127);
MIDIMessage b9m(midi::NoteOn, NOTE_Gb3, 127);
MIDIMessage p1m(midi::ControlChange, midi::BreathController, 0);
MIDIMessage p2m(midi::ControlChange, midi::BreathController, 0);
MIDIMessage p3m(midi::ControlChange, midi::BreathController, 0);

// SEQUENCE DATA
Step s1(NOTE_C_1, 1, 0);
Step s2(NOTE_D_1, 1, 0);
Step s3(NOTE_E_1, 1, 0);
Step s4(NOTE_F_1, 1, 0);
Step s5(NOTE_G_1, 1, 0);
Step s6(NOTE_A_1, 1, 0);
Step s7(NOTE_B_1, 1, 0);
Step s8(NOTE_C0, 1, 0);

uint16_t address = 0;

uint8_t pageSize = (MIDI_BUTTONS_NUM * MIDI_BUTTONS_SIZE) + (MIDI_POTS_NUM * MIDI_POTS_SIZE);
uint8_t sequenceSize = (SEQUENCE_LENGTH * STEP_SIZE);


void setup(void)
{
  Serial.begin(9600);

  // STORE GLOBAL CONFIG DATA
  EEPROM.update(address, _config.getMIDIChannel());
  address += sizeof(uint8_t);
  EEPROM.update(address, _config.getSequencerMIDIChannel());
  address += sizeof(uint8_t);
  EEPROM.update(address, _config.getMode());
  address += sizeof(uint8_t);
  EEPROM.update(address, _config.getRootNote());
  address += sizeof(uint8_t);
  EEPROM.update(address, _config.getSendClockWhilePlayback());
  address += sizeof(uint8_t);
	
  // STORE PAGES DATA
  for (int i = 0; i < NUM_PAGES; i++)
  {  
    saveMIDIMessage(&address, b1m); 
    saveMIDIMessage(&address, b2m);
    saveMIDIMessage(&address, b3m);
    saveMIDIMessage(&address, b4m);
    saveMIDIMessage(&address, b5m);
    saveMIDIMessage(&address, b6m);
    saveMIDIMessage(&address, b7m);
    saveMIDIMessage(&address, b8m);
    saveMIDIMessage(&address, b9m);

    saveMIDIMessage(&address, p1m);
    saveMIDIMessage(&address, p2m);
    saveMIDIMessage(&address, p3m);
  }
  
  // STORE SEQUENCES DATA
  for (int i = 0; i < NUM_SEQUENCES; i++)
  { 
    
    if (i==0)
    {      
      saveStep(&address, s1);    
      saveStep(&address, s2);
      saveStep(&address, s3);
      saveStep(&address, s4);
      saveStep(&address, s5);
      saveStep(&address, s6);
      saveStep(&address, s7);
      saveStep(&address, s8);
    }

    else
    {
      s1.setNote(s1.getNote() + 12);
      saveStep(&address, s1);
    
      s2.setNote(s2.getNote() + 12);
      saveStep(&address, s2);
    
      s3.setNote(s3.getNote() + 12);
      saveStep(&address, s3);
 
      s4.setNote(s4.getNote() + 12);
      saveStep(&address, s4);
  
      s5.setNote(s5.getNote() + 12);
      saveStep(&address, s5);
    
      s6.setNote(s6.getNote() + 12);
      saveStep(&address, s6);
    
      s7.setNote(s7.getNote() + 12);
      saveStep(&address, s7);
    
      s8.setNote(s8.getNote() + 12);
      saveStep(&address, s8);
    } 
 
  }
  
  // LOAD THE REST OF THE MEMORY SLOTS WITH DEFAULT VALUE
  while (address < MEMORY_SIZE)
  {
	EEPROM.update(address,-1);
	address += sizeof(uint8_t);
  } 

  address = 0;
   
  // Print the stored global configuration
  Serial.println("MIDI CHANNEL:");
  Serial.println(EEPROM.read(address), DEC);
  address++;
  
  Serial.println("SEQUENCER MIDI CHANNEL:");
  Serial.println(EEPROM.read(address), DEC);
  address++;

  Serial.println("MODE:");
  Serial.println(EEPROM.read(address), DEC);
  address++;

  Serial.println("ROOT NOTE");
  Serial.println(EEPROM.read(address), DEC);
  address++;

  Serial.println("SEND CLOCK WHILE PLAYBACK");
  Serial.println(EEPROM.read(address), DEC);
  address++;

  // Print the pages stored messages
  for (int i = 0; i < NUM_PAGES * ((MIDI_BUTTONS_NUM * MIDI_BUTTONS_SIZE) + (MIDI_POTS_NUM * MIDI_POTS_SIZE)); i++)
  {
    if (i % 3 == 0)
    {
      Serial.print("MESSAGE: ");
      Serial.println(i / 3, DEC);
    }
   
    Serial.println(EEPROM.read(address), DEC);
    address++;
  }
  
  // Print the sequencers stored steps  
  for (int i = 0; i < (NUM_SEQUENCES * SEQUENCE_LENGTH) * STEP_SIZE; i++)
  {
    if (i % 3 == 0)
    {
      Serial.print("STEP: ");
      Serial.println(i / 3, DEC);
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
}

void saveStep(uint16_t * address, Step step)
{
   EEPROM.update((*address),step.getNote());
   (*address) += sizeof(uint8_t);
   EEPROM.update((*address),step.isEnabled());
   (*address) += sizeof(uint8_t);
   EEPROM.update((*address),step.isLegato());
   (*address) += sizeof(uint8_t);  
}

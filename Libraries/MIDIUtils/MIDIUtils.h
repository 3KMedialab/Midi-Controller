/*
 * MIDIUtils.h
 *
 * Helper class for MIDI messages manipulation 
 *
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

#ifndef MIDIUtils_h
#define MIDIUtils_h

#include <Arduino.h>
#include <avr/pgmspace.h>

#define IONINAN 0
#define DORIAN 1
#define PHRYGIAN 2
#define LYDIAN 3
#define MIXOLYDIAN 4
#define AEOLIAN 5
#define LOCRIAN 6
#define MAJBLUES 7
#define MINBLUES 8
#define CHROMATIC 9
#define ERROR 10

#define NOTE_C 11
#define NOTE_Db 12
#define NOTE_D 13
#define NOTE_Eb 14
#define NOTE_E 15
#define NOTE_F 16
#define NOTE_Gb 17
#define NOTE_G 18
#define NOTE_Ab 19
#define NOTE_A 20
#define NOTE_Bb 21
#define NOTE_B 22

const char msg_Ionian[] PROGMEM = "Ionian";
const char msg_Dorian[] PROGMEM = "Dorian";
const char msg_Phrygian[] PROGMEM = "Phrygian";
const char msg_Lydian[] PROGMEM = "Lydian";
const char msg_Mixolydian[] PROGMEM = "Mixolydian";
const char msg_Aeolian[] PROGMEM = "Aeolian";
const char msg_Locrian[] PROGMEM = "Locrian";
const char msg_MajBlues[] PROGMEM = "Maj Blues";
const char msg_MinBlues[] PROGMEM = "Min Blues";
const char msg_Chromatic[] PROGMEM = "Chromatic";
const char msg_Error[] PROGMEM = "N/A";
const char msg_C[] PROGMEM = "C";
const char msg_Db[] PROGMEM = "Db";
const char msg_D[] PROGMEM = "D";
const char msg_Eb[] PROGMEM = "Eb";
const char msg_E[] PROGMEM = "E";
const char msg_F[] PROGMEM = "F";
const char msg_Gb[] PROGMEM = "Gb";
const char msg_G[] PROGMEM = "G";
const char msg_Ab[] PROGMEM = "Ab";
const char msg_A[] PROGMEM = "A";
const char msg_Bb[] PROGMEM = "Bb";
const char msg_B[] PROGMEM = "B";

const char * const midiStrings[] PROGMEM = {msg_Ionian, msg_Dorian, msg_Phrygian, msg_Lydian, msg_Mixolydian, msg_Aeolian, msg_Locrian, msg_MajBlues, 
msg_MinBlues, msg_Chromatic, msg_Error, msg_C, msg_Db, msg_D, msg_Eb, msg_E, msg_F, msg_Gb, msg_G, msg_Ab, msg_A, msg_Bb, msg_B};

class MIDIUtils
{
  public:

    // Root keys
    enum {C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B};
     
    // Musical modes
    enum {Ionian, Dorian, Phrygian, Lydian, Mixolydian, Aeolian, Locrian, Maj_Blues, Min_Blues, Chromatic};

    // MIDI channels
    enum {CHANNEL1 = 1, CHANNEL2, CHANNEL3, CHANNEL4, CHANNEL5, CHANNEL6, CHANNEL7, CHANNEL8, CHANNEL9, CHANNEL10, CHANNEL11, CHANNEL12, CHANNEL13, CHANNEL14, CHANNEL15, CHANNEL16};

    static int8_t getOctave(uint8_t midiNote);
    static uint8_t getNoteNumber(uint8_t midiNote);
    static char * getNoteName(uint8_t midiNote);
    static uint8_t isNoteInScale(uint8_t midiNote, uint8_t rootNote, uint8_t mode);     
    static char * getModeName(uint8_t mode);    

  private:
    static void getMessage(uint8_t msgIndex, char * buffer);
};
#endif
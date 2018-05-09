/*
 * MIDIUtils.cpp
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

#include <MIDIUtils.h>

// Musical modes intervals
const uint8_t intervals [7][7] = {

    {0, 2, 4, 5, 7, 9, 11}, // Ionian
    {0, 2, 3, 5, 7, 9, 10}, // Dorian
    {0, 1, 3, 5, 7, 8, 10}, // Phrygian
    {0, 2, 4, 6, 7, 8, 10}, // Lydian
    {0, 2, 4, 5, 7, 9, 10}, // Mixolydian
    {0, 2, 3, 5, 7, 8, 10}, // Aeolian
    {0, 1, 3, 5, 6, 8, 10}  // Locrian
};

/*
* Return the octave of a note
* midiNote: note which octave will be returned
*/
String MIDIUtils::getOctave(uint8_t midiNote)
{
     //Calculate the octave of the midi note
     String octave = String((midiNote / 12) -1, DEC);
     return octave;
}

/*
* Return the number within a scale of a note
* midiNote: note which number will be returned
*/
uint8_t MIDIUtils::getNoteNumber(uint8_t midiNote)
{
    //Calculate the midi note value (0-11) 
    return midiNote % 12;
}

/*
* Return the note name of a note
* midiNote: note which name will be returned
*/
String MIDIUtils::getNoteName(uint8_t midiNote)
{
    //Get note number (0-11) 
    uint8_t note = getNoteNumber(midiNote);
    
    //Use a switch statement to determine note name. This could also
    //be done with a series of "if" statements. 
    switch(note)
    {
      //Note: each case returns so break keyword is not needed here
       case C:   return F("C");    
       case Db:  return F("Db");
       case D:   return F("D"); 
       case Eb:  return F("Eb"); 
       case E:   return F("E"); 
       case F:   return F("F"); 
       case Gb:  return F("Gb"); 
       case G:   return F("G"); 
       case Ab:  return F("Ab"); 
       case A:   return F("A"); 
       case Bb:  return F("Bb");  
       case B:   return F("B");
       default:  return F("N/A");        
    }
}

/*
* Return true if a note belongs to a scale. False otherwise
* midiNote: note to determine if belongs to a scale
* rootNote: is the key of the scale
* mode: musical mode of the scale
*
* EXAMPLE: how to generate G Aeolian scale
* rootNote = 7
* mode = Aeolian
* resulting scale = ((7+0)%12, (7+2)%12, (7+3)%12, (7+5)%12, (7+7)%12, (7+8)%12, (7+10)%12) -> (7, 9, 10, 0, 2, 3, 5) 
*/
uint8_t MIDIUtils::isNoteInScale(uint8_t midiNote, uint8_t rootNote, uint8_t mode)
{
    if (mode == Chromatic)
    {
        return 1;
    }        

    for (int i=0; i<7; i++)
    {
        if ( (rootNote + intervals[mode][i]) % 12 == midiNote % 12 )
        {
            return 1;
        }
    }

    return 0;
}
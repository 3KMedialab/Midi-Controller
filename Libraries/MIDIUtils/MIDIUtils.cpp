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
#include <avr/pgmspace.h>

// Musical modes intervals
const uint8_t intervals [9][7] PROGMEM = {

    {0, 2, 4, 5, 7, 9, 11}, // Ionian
    {0, 2, 3, 5, 7, 9, 10}, // Dorian
    {0, 1, 3, 5, 7, 8, 10}, // Phrygian
    {0, 2, 4, 6, 7, 8, 10}, // Lydian
    {0, 2, 4, 5, 7, 9, 10}, // Mixolydian
    {0, 2, 3, 5, 7, 8, 10}, // Aeolian
    {0, 1, 3, 5, 6, 8, 10}, // Locrian
    {0, 2, 3, 4, 7, 9, 0},  // Major Blues
    {0, 3, 5, 6, 7, 10, 0}  // Minor Blues
};

/*
* Return the octave of a note
* midiNote: note which octave will be returned
*/
int8_t MIDIUtils::getOctave(uint8_t midiNote)
{
    //Calculate the octave of the midi note
    return (midiNote / 12) -1;     
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
char *  MIDIUtils::getNoteName(uint8_t midiNote)
{
    char buffer [4];

    //Get note number (0-11) 
    uint8_t note = getNoteNumber(midiNote);
    
    //Use a switch statement to determine note name. This could also
    //be done with a series of "if" statements. 
    switch(note)
    {
      //Note: each case returns so break keyword is not needed here
       case C:   
        getMessage(NOTE_C, buffer); 
        break;    
       
       case Db:  
        getMessage(NOTE_Db, buffer); 
        break;

       case D:   
        getMessage(NOTE_D, buffer); 
        break; 
       
       case Eb:  
        getMessage(NOTE_Eb, buffer); 
        break;

       case E:
        getMessage(NOTE_E, buffer); 
        break;

       case F:   
        getMessage(NOTE_F, buffer); 
        break; 
       
       case Gb: 
        getMessage(NOTE_Gb, buffer); 
        break;

       case G:  
        getMessage(NOTE_G, buffer); 
        break;

       case Ab:  
        getMessage(NOTE_Ab, buffer); 
        break;

       case A:   
        getMessage(NOTE_A, buffer); 
        break;

       case Bb:  
        getMessage(NOTE_Bb, buffer); 
        break;  
       
       case B:  
        getMessage(NOTE_B, buffer); 
        break;

       default:  
        getMessage(ERROR, buffer);                 
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
        if ( (rootNote + pgm_read_byte (&(intervals[mode][i]))) % 12 == midiNote % 12 )
        {
            return 1;
        }
    }

    return 0;
}

/*
* Return the name of a musical mode
* mode: mode number which name will be returned
*/
char * MIDIUtils::getModeName(uint8_t mode)
{
    char buffer [12];

    //Use a switch statement to determine mode name. 
    switch(mode)
    {
      //Note: each case returns so break keyword is not needed here
     
       case Ionian:     
        getMessage(IONINAN, buffer); 
        break;

       case Dorian:     
        getMessage(DORIAN, buffer); 
        break;

       case Phrygian:   
        getMessage(PHRYGIAN, buffer); 
        break;

       case Lydian:     
        getMessage(LYDIAN, buffer); 
        break;

       case Mixolydian: 
        getMessage(MIXOLYDIAN, buffer); 
        break;

       case Aeolian:    
        getMessage(AEOLIAN, buffer); 
        break;

       case Locrian:    
        getMessage(LOCRIAN, buffer); 
        break;

       case Maj_Blues:  
        getMessage(MAJBLUES, buffer);
        break; 
       
       case Min_Blues:  
        getMessage(MINBLUES, buffer); 
        break;

       case Chromatic:  
        getMessage(CHROMATIC, buffer); 
        break;

       default:         
        getMessage(ERROR, buffer);    
    }

    return buffer;
}

/*
* Load a string of chars from the PROGMEM
* msgIndex: text to Load
* buffer: array of chars where the text will be loaded
*/
void MIDIUtils::getMessage(uint8_t msgIndex, char * buffer)
{
    strcpy_P(buffer, (char*)pgm_read_word(&(midiStrings[msgIndex])));    
}
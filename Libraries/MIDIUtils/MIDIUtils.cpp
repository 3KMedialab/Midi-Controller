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

uint8_t MIDIUtils::getOctave(uint8_t midiNote)
{
     //Calculate the octave of the midi note
     return (midiNote / 12) -1;
}

uint8_t MIDIUtils::getNoteNumber(uint8_t midiNote)
{
    //Calculate the midi note value (0-11) 
    return midiNote % 12;
}

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
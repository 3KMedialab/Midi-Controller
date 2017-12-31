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

class MIDIUtils
{
  public:
    static String getOctave(uint8_t midiNote);
    static uint8_t getNoteNumber(uint8_t midiNote);
    static String getNoteName(uint8_t midiNote);    

  private:
    enum{C, Db, D, Eb, E, F, Gb, G, Ab, A, Bb, B};
};
#endif
/*
 * Step.h
 *
 * This class represents a Step within a sequence. Each step is composed of the following attributes:
 * Note: is the MIDI note that will be played
 * Enabled: wether the note should be played or not.
 * legato: wether the note should be played legato with the foloowing step
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

#ifndef Step_h
#define Step_h

#include "Arduino.h"

class Step
{
  public:   
  
  Step();
	Step (uint8_t note, uint8_t enabled, uint8_t legato);
	
    uint8_t getNote();
    uint8_t isEnabled();
    uint8_t isLegato();

    void setNote (uint8_t note);
    void setEnabled(uint8_t enabled);
    void setLegato(uint8_t legato);    
	
	static uint8_t getSize();

  private:

    uint8_t _note;
    uint8_t _enabled;
    uint8_t _legato;
  
};
#endif
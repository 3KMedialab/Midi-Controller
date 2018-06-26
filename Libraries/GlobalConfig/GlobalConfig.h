/*
 * GlobalConfig.h
 *
 * Class that contains the Global Configuration attributes
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
#ifndef GlobalConfig_h
#define GlobalConfig_h

#include "Arduino.h"

class GlobalConfig
{
    public:
        
        GlobalConfig (uint8_t MIDIChannel, uint8_t sequencerMIDIChannel, uint8_t mode, uint8_t rootNote);
        GlobalConfig ();
        void setMIDIChannel (uint8_t MIDIChannel);
		void setSequencerMIDIChannel (uint8_t sequencerMIDIChannel);
        void setMode (uint8_t mode);
        void setRootNote (uint8_t rootNote);
        
        uint8_t getMIDIChannel();
		uint8_t getSequencerMIDIChannel();
        uint8_t getMode();
        uint8_t getRootNote();

        uint8_t getSize();
    
    private:             
        uint8_t _MIDIChannel;
		uint8_t _sequencerMIDIChannel;
        uint8_t _mode;
        uint8_t _rootNote;       
};
#endif
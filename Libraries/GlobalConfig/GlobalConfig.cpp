/*
 * GlobalConfig.cpp
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

#include <GlobalConfig.h>

/*
* Constructor
*/
GlobalConfig::GlobalConfig(uint8_t MIDIChannel, uint8_t mode, uint8_t rootNote)
{
     _MIDIChannel = MIDIChannel;   
     _mode = mode; 
     _rootNote = rootNote;
}

/*
* Constructor
*/
GlobalConfig::GlobalConfig(){}

/*
* Setter method
*/
void GlobalConfig::setMIDIChannel(uint8_t MIDIChannel)
{
    _MIDIChannel = MIDIChannel;
}

/*
* Setter method
*/
void GlobalConfig::setMode(uint8_t mode)
{
    _mode = mode;
}

/*
* Setter method
*/
void GlobalConfig::setRootNote(uint8_t rootNote)
{
    _rootNote = rootNote;
}

/*
* Getter method
*/
uint8_t GlobalConfig::getMIDIChannel()
{
    return _MIDIChannel;    
}

/*
* Getter method
*/
uint8_t GlobalConfig::getMode()
{
    return _mode;    
}

/*
* Getter method
*/
uint8_t GlobalConfig::getRootNote()
{
    return _rootNote;    
}

/*
* Return the size in bytes of this class
*/
static uint8_t GlobalConfig::getSize()
{
    return sizeof(uint8_t) * 3;    
}
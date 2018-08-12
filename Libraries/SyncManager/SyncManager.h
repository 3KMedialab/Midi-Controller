/*
 * SyncManager.h
 *
 * Class that manage the global system tempo, which is used by the MIDI controller and the sequencer 
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

#ifndef SyncManager_h
#define SyncManager_h

#include <Arduino.h>

class SyncManager
{
  public:    

  uint16_t getBpm ();

  void setBpm(uint16_t bpm);

  private:

    uint16_t _bpm;

};
#endif
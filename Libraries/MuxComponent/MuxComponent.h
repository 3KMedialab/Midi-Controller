/*
 * MuxComponent.h
 *
 * Class that represens a component connected to Arduino through a Multiplexer
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

#ifndef MuxComponent_h
#define MuxComponent_h

#include "Multiplexer.h"

class MuxComponent
{
  public:
    MuxComponent(Multiplexer * mux, uint8_t channel);      

  protected:
    Multiplexer * _mux;
    uint8_t _channel;           
};
#endif
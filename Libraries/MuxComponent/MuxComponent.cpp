/*
 * MuxComponent.cpp
 *
 * Class that represents a component connected to Arduino through a Multiplexer
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

#include "MuxComponent.h"

/*
* Constructor
* mux: multiplexer where the component is connected to
* channel: input of the multiplexer where the component is connected
*/
MuxComponent::MuxComponent(Multiplexer * mux, uint8_t channel)
{
  _mux = mux;
  _channel = channel;
}


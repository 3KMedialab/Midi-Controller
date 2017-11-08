/*
 * ScreenManager.h
 *
 * Class that manage the interactions between the MIDI controller and the screen
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
#ifndef ScreenManager_h
#define ScreenManager_h

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <avr/pgmspace.h>

#define MSG_PAGE 0
#define MSG_TEMPO 1
#define MSG_BPM 2

// Messages that will be displayed on the screen
const char msg_Page[] PROGMEM = "Page: ";
const char msg_Tempo[] PROGMEM = "Tempo: ";
const char msg_Bpm[] PROGMEM = " BPM";

const char * const messages[] PROGMEM = {msg_Page, msg_Tempo, msg_Bpm};

class ScreenManager
{
  public:   
    void initialize(uint8_t i2cAddress, uint8_t rowLength, uint8_t rows);
    void printDefault(uint8_t page, uint16_t tempo);
    void cleanScreen();

  private:
    void getMessage(uint8_t msgIndex, char * buffer);
    
    LiquidCrystal_I2C _screen;     
};
#endif
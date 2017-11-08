/*
 * ScreenManager.cpp
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

#include "ScreenManager.h"

/*
* Initializes the screen manager
* i2cAddress	I2C slave address of the LCD display. Most likely printed on the
*			    LCD circuit board, or look in the supplied LCD documentation.
* cols          Number of columns your LCD display has.
* rows	        Number of rows your LCD display has.
*/
void ScreenManager::initialize(uint8_t i2cAddress, uint8_t cols, uint8_t rows)
{
    _screen.setLCDAddress(i2cAddress);
    _screen.setLCDCols(cols);
    _screen.setLCDRows(rows);

    _screen.begin();
}

void ScreenManager::printDefault(uint8_t page, uint16_t tempo)
{
    char buffer[10];
    uint8_t aux = 0;
    String s ="";
    
    //Set the cursor on the top left of the screen
    _screen.home();
    
    // prints the loaded page information
    getMessage(MSG_PAGE, buffer);  
    _screen.print(buffer);
    aux += strlen(buffer);
    _screen.print(page, DEC);
    s.concat(page);
    aux += s.length();
    for (int i=aux;i<_screen.getLCDCols();i++)
    {
        _screen.print(" ");    
    }

    aux = 0;
    s = "";

    // prints the tempo information
    _screen.setCursor(0,1);
     getMessage(MSG_TEMPO, buffer);  
    _screen.print(buffer);
    aux += strlen(buffer);
    _screen.print(tempo, DEC);

    s.concat(tempo);
    aux += s.length();
   
    getMessage(MSG_BPM, buffer);  
    _screen.print(buffer);
    aux += strlen(buffer);
    for (int i=aux;i<_screen.getLCDCols();i++)
    {
        _screen.print(" ");    
    }
}

void ScreenManager::getMessage(uint8_t msgIndex, char * buffer)
{
    strcpy_P(buffer, (char*)pgm_read_word(&(messages[msgIndex])));    
}

void ScreenManager::cleanScreen()
{
    _screen.clear();
}
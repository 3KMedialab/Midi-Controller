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

    _displayedMIDIComponent = NULL;
    _currentMIDIMessageDisplayed = 0;

    _screen.begin();
}

void ScreenManager::printDefault(uint8_t page, uint16_t tempo)
{
    char buffer[10];
    uint8_t aux = 0;
    String s ="";

    // No MIDI component is assigned to the Screen Manager
    _displayedMIDIComponent = NULL;
    
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

void ScreenManager::printSelectComponentMessage()
{
    char buffer[20]; 

    _screen.noBlink();
    cleanScreen();

    getMessage(MSG_EDIT, buffer);  
    for (int i=0; i<strlen(buffer); i++)
    {
        if (buffer[i] != '\n')
        {
            _screen.write(buffer[i]);
        }
        else
        {
            _screen.setCursor(0,1);
        }
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

IMIDIComponent * ScreenManager::getDisplayedMIDIComponent()
{
    return _displayedMIDIComponent;
}

uint8_t ScreenManager::getDisplayedMessageIndex()
{
    return _currentMIDIMessageDisplayed;
}

void ScreenManager::displayComponentMIDIMessage(uint8_t msgIndex)
{
    char buffer[20];

    if (_displayedMIDIComponent != NULL)
    {    
        // set the currently MIDI message being displayed
        _currentMIDIMessageDisplayed = msgIndex;
        
        cleanScreen();

        // display current message index and total messages of the component
        _screen.print(msgIndex);
        _screen.print(F("/"));
        _screen.print(_displayedMIDIComponent->getNumMessages());
        _screen.print(F(" "));

        // display the first MIDI message of the component
        switch((_displayedMIDIComponent->getMessages()[msgIndex-1]).getType())
        {
            case midi::NoteOn:
                getMessage(MSG_NOTE_ON, buffer);  
                _screen.print(buffer);
                _screen.setCursor(0,1);
                printNoteOnOffMIDIMessage(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;

            case midi::NoteOff:
                getMessage(MSG_NOTE_OFF, buffer);  
                _screen.print(buffer);
                _screen.setCursor(0,1);
                printNoteOnOffMIDIMessage(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;

            case midi::ControlChange:
                getMessage(MSG_CTRL_CHANGE, buffer);  
                _screen.print(buffer);
                _screen.setCursor(0,1);
                printCCMIDIMessage(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;

            case midi::ProgramChange:
                getMessage(MSG_PGRM_CHANGE, buffer);  
                _screen.print(buffer);
                _screen.setCursor(0,1);
                printPCMIDIMessage(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;
        }

        // display cursor for editing the message getType
        _screen.setCursor(4,0);
        _screen.blink();
    }    
}

void ScreenManager::printNoteOnOffMIDIMessage(MIDIMessage message)
{
    char buffer[10]; 
    
    //print Note
    String note = String(MIDIUtils::getNoteName(message.getDataByte1()));
    note.concat(MIDIUtils::getOctave(message.getDataByte1()));
    note.concat(F(" "));
    _screen.print(note);

    //print velocity
    getMessage(MSG_VELOCITY, buffer);  
    _screen.print(buffer);
    _screen.print(message.getDataByte2());
    _screen.print(F(" "));   
    
    //print channel
    printMIDIChannel(message.getChannel());
}

void ScreenManager::printCCMIDIMessage(MIDIMessage message)
{
    char buffer[10];

    //print CC Number
    getMessage(MSG_CC, buffer);  
    _screen.print(buffer);
    _screen.print(message.getDataByte1());
    _screen.print(F(" "));   

    //print CC value
    _screen.print(message.getDataByte2());
    _screen.print(F(" "));   

    //print channel
    printMIDIChannel(message.getChannel());
}

void ScreenManager::printPCMIDIMessage(MIDIMessage message)
{
    char buffer[10];

    //print program Number
    getMessage(MSG_PGM, buffer);  
    _screen.print(buffer);
    _screen.print(message.getDataByte1());
    _screen.print(F(" "));       

    //print channel
    printMIDIChannel(message.getChannel());
}

void ScreenManager::printMIDIChannel(uint8_t channel)
{
    char buffer[4];

    getMessage(MSG_CHANNEL, buffer);  
    _screen.print(buffer);
    _screen.print(channel);  
}

uint8_t ScreenManager::isComponentDisplayed()
{
    return _displayedMIDIComponent == NULL ? 0 : 1;  
}

void ScreenManager::displayPreviousMIDIMsg()
{
    if (_currentMIDIMessageDisplayed > 1)
    {
        _currentMIDIMessageDisplayed -= 1;
        displayComponentMIDIMessage(_currentMIDIMessageDisplayed);
    }
}

void ScreenManager::displayNextMIDIMsg()
{
    if (_currentMIDIMessageDisplayed < _displayedMIDIComponent->getNumMessages())
    {
        _currentMIDIMessageDisplayed += 1;
        displayComponentMIDIMessage(_currentMIDIMessageDisplayed);
    }    
}

void ScreenManager::setMIDIComponentToDisplay(IMIDIComponent * midiComponent)
{
    _displayedMIDIComponent = midiComponent;
}

void ScreenManager::displayMIDIMessageType(uint8_t midiMessageType)
{
    char buffer[15];

    // calculate the cursor position of the beginning of the MIDI message type name
    String aux = String(_currentMIDIMessageDisplayed,DEC);
    aux.concat(F("/"));
    aux.concat(_displayedMIDIComponent->getNumMessages());
    aux.concat(F(" "));

    uint8_t backCursorPosition = aux.length();

    switch(midiMessageType)
    {
        case midi::NoteOn:
            getMessage(MSG_NOTE_ON, buffer);             
        break;

        case midi::NoteOff:
            getMessage(MSG_NOTE_OFF, buffer);                    
        break;

        case midi::ControlChange:
            getMessage(MSG_CTRL_CHANGE, buffer);                   
        break;

        case midi::ProgramChange:
            getMessage(MSG_PGRM_CHANGE, buffer);                   
        break;
    }
    
    // print hte MIDI message type name on screen
    _screen.print(buffer);
    aux.concat(buffer);

    // clean the rest of the line
    for (int i=aux.length();i<_screen.getLCDCols();i++)
    {
        _screen.print(" ");    
    }

    // set the cursor at the beginning of the MIDI message type name
    _screen.setCursor(backCursorPosition,0);
}

void ScreenManager::printSavedMessage()
{
    char buffer[15];
    
    cleanScreen();
    _screen.noBlink();   
    getMessage(MSG_SAVED, buffer);  
    _screen.print(buffer);    
}
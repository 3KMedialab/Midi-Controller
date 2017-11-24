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
    String s ="";

    // No MIDI component is assigned to the Screen Manager
    _displayedMIDIComponent = NULL;
    
    //Set the cursor on the top left of the screen
    _screen.home();
    
    // prints the loaded page information
    getMessage(MSG_PAGE, buffer); 
    s.concat(buffer); 
    s.concat(page);
    _screen.print(s);
       
    clearRangeOnCurentLine(0, s.length(), _screen.getLCDCols());
    
    s = "";

    // prints the tempo information
    _screen.setCursor(0,1);
    getMessage(MSG_TEMPO, buffer);  
    s.concat(buffer);
    s.concat(tempo);   
    getMessage(MSG_BPM, buffer);  
    s.concat(buffer);
    _screen.print(s);

    clearRangeOnCurentLine(1, s.length(), _screen.getLCDCols());    
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

uint8_t ScreenManager::getDisplayedMessageType()
{
    if (_displayedMIDIComponent != NULL)
    {
        return _displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1].getType();
    }
    else
    {
        return midi::InvalidType; 
    }
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
                printNoteOnOffMIDIData(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;

            case midi::NoteOff:
                getMessage(MSG_NOTE_OFF, buffer);  
                _screen.print(buffer);
                printNoteOnOffMIDIData(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;

            case midi::ControlChange:
                getMessage(MSG_CTRL_CHANGE, buffer);  
                _screen.print(buffer);         
                printCCMIDIData(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;

            case midi::ProgramChange:
                getMessage(MSG_PGRM_CHANGE, buffer);  
                _screen.print(buffer);   
                printPCMIDIData(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;
        }

        // display cursor for editing the message getType
        _screen.setCursor(4,0);
        _screen.blink();
    }    
}

void ScreenManager::printNoteOnOffMIDIData(MIDIMessage message)
{
    char buffer[10];

    //print note    
    _screen.setCursor(NOTE_POS,1);
    String aux = String(MIDIUtils::getNoteName(message.getDataByte1()));
    aux.concat(MIDIUtils::getOctave(message.getDataByte1()));
    aux.concat(F(" "));    
    _screen.print(aux);
      
    //print velocity
    _screen.setCursor(VELOCITY_POS,1);
    getMessage(MSG_VELOCITY, buffer);  
    aux= String(buffer);
    aux.concat(message.getDataByte2());
    _screen.print(aux);   
   
    //print channel
    _screen.setCursor(NOTE_ON_OFF_CHANNEL_POS,1);
    printMIDIChannel(message.getChannel());
}

void ScreenManager::printCCMIDIData(MIDIMessage message)
{
    char buffer[10];
    String aux = "";

    //print CC Number
    _screen.setCursor(CC_POS,1);   
    getMessage(MSG_CC, buffer);  
    aux.concat(buffer);
    aux.concat(message.getDataByte1());
    _screen.print(aux);
    
    //print channel
    _screen.setCursor(CC_CHANNEL_POS,1);  
    printMIDIChannel(message.getChannel());
}

void ScreenManager::printPCMIDIData(MIDIMessage message)
{
    char buffer[10];    

    //print channel
    _screen.setCursor(PROGRAM_CHANNEL_POS,1); 
    printMIDIChannel(message.getChannel());     
}

void ScreenManager::printMIDIChannel(uint8_t channel)
{
    char buffer[4];
    String aux = "";

    getMessage(MSG_CHANNEL, buffer);  
    aux.concat(buffer);
    aux.concat(channel);
    _screen.print(aux);   
    
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

void ScreenManager::refreshMIDIData()
{
    char buffer[15];

    // calculate the cursor position of the beginning of the MIDI message type name
    String aux = String(_currentMIDIMessageDisplayed,DEC);
    aux.concat(F("/"));
    aux.concat(_displayedMIDIComponent->getNumMessages());
    aux.concat(F(" "));

    uint8_t backCursorPosition = aux.length();

    switch(_displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1].getType())
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

    // clean the rest of the line and refresh the MIDI data displayed
    aux.concat(buffer);
    clearRangeOnCurentLine(0, aux.length(), _screen.getLCDCols());
    clearRangeOnCurentLine(1, 0, _screen.getLCDCols());
 
    switch (_displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1].getType())
    {
        case midi::NoteOn:
            printNoteOnOffMIDIData(_displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1]);            
        break;

        case midi::NoteOff:
            printNoteOnOffMIDIData(_displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1]);    
        break;

        case midi::ControlChange:
            printCCMIDIData(_displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1]);                    
        break;

        case midi::ProgramChange:
            printPCMIDIData(_displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1]);                  
        break;
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

void ScreenManager::moveCursorToMsgType()
{
    String aux = String(_currentMIDIMessageDisplayed,DEC);
    aux.concat(F("/"));
    aux.concat(_displayedMIDIComponent->getNumMessages());
    aux.concat(F(" "));
    _screen.setCursor(aux.length(),0);
}

void ScreenManager::moveCursorToNote()
{
    _screen.setCursor(NOTE_POS,1); 
}

void ScreenManager::moveCursorToVelocity()
{
    char buffer[10];    

    getMessage(MSG_CC, buffer);  
    _screen.setCursor(VELOCITY_POS + strlen(buffer) - 1,1);   
}

void ScreenManager::moveCursorToCC()
{
    char buffer[10];
    
    getMessage(MSG_CC, buffer);  
    _screen.setCursor(strlen(buffer),1);    
}

void ScreenManager::moveCursorToChannel()
{
    char buffer[10]; 

    // set the cursor regarding the MIDImessage type
    switch(_displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1].getType())
    {
        case midi::NoteOn:
        case midi::NoteOff:
              
            getMessage(MSG_CHANNEL, buffer);  
            _screen.setCursor(NOTE_ON_OFF_CHANNEL_POS + strlen(buffer),1);
        
        break;
    
       
        case midi::ControlChange:
                        
            getMessage(MSG_CHANNEL, buffer);
            _screen.setCursor(CC_CHANNEL_POS + strlen(buffer),1);
        
        break;
                

        case midi::ProgramChange:

            getMessage(MSG_CHANNEL, buffer);
            _screen.setCursor(PROGRAM_CHANNEL_POS + strlen(buffer),1);        
          
        break;    
    }
}


void ScreenManager::refreshNoteValue (uint8_t note)
{
    _screen.noBlink();
    
    String noteName = String(MIDIUtils::getNoteName(note));
    noteName.concat(MIDIUtils::getOctave(note));   
    _screen.print(noteName);

    clearRangeOnCurentLine(1,noteName.length(),VELOCITY_POS);
    moveCursorToNote();

    _screen.blink();
}

void ScreenManager::refreshVelocityValue(uint8_t velocity)
{
    char buffer[5];

    _screen.noBlink();
       
    String velocityValue= String (velocity, DEC);
    _screen.print(velocityValue);
 
    getMessage(MSG_VELOCITY, buffer);
    clearRangeOnCurentLine(1, VELOCITY_POS + strlen(buffer) + velocityValue.length(), NOTE_ON_OFF_CHANNEL_POS);
    moveCursorToVelocity();

    _screen.blink();
}

void ScreenManager::refreshCCValue(uint8_t cc)
{
    char buffer[5];
    
    _screen.noBlink();
    
    String ccValue= String (cc, DEC);
    _screen.print(ccValue);

    getMessage(MSG_CC, buffer);
    clearRangeOnCurentLine(1, CC_POS + strlen(buffer) + ccValue.length(), CC_CHANNEL_POS);
    moveCursorToCC();

    _screen.blink();
}

void ScreenManager::refreshChannelValue(uint8_t channel)
{    
    char buffer[5];

    _screen.noBlink();

    String channelValue = String (channel, DEC);
    _screen.print(channelValue);
    
    getMessage(MSG_CHANNEL, buffer);

    // clean line regarding the MIDImessage type
    switch(_displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1].getType())
    {
        case midi::NoteOn:
        case midi::NoteOff:
            clearRangeOnCurentLine(1, NOTE_ON_OFF_CHANNEL_POS + strlen(buffer) + channelValue.length(), _screen.getLCDCols()); 
        break;    
       
        case midi::ControlChange:
            clearRangeOnCurentLine(1, CC_CHANNEL_POS + strlen(buffer) + channelValue.length(), _screen.getLCDCols());   
        break;                

        case midi::ProgramChange:
            clearRangeOnCurentLine(1, PROGRAM_CHANNEL_POS + strlen(buffer) + channelValue.length(), _screen.getLCDCols()); 
        break;    
    }
    
    moveCursorToChannel();

    _screen.blink();
}

void ScreenManager::clearRangeOnCurentLine(uint8_t row, uint8_t from, uint8_t to)
{
    _screen.setCursor(from, row);
    for (int i=from; i<to; i++)
    {
        _screen.print(F(" "));
    }
}
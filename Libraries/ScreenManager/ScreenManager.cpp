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
void ScreenManager::initialize()
{
    _lcd.begin(COLUMNS,ROWS);               // initialize the lcd   

    Wire.setClock(400000L);

    _displayedMIDIComponent = NULL;
    _currentMIDIMessageDisplayed = 0;
}

/*
* Prints the information of controller's pages and selected tempo
* page: current selected page of messages
* tempo: current selected tempo in BPMs
*/
void ScreenManager::printDefault(uint8_t page, uint8_t numPages, uint16_t tempo, uint8_t isMIDIClockOn)
{
    char line[COLUMNS+1];

    // No MIDI component is assigned to the Screen Manager
    _displayedMIDIComponent = NULL;
       
    //Set the cursor on the top left of the screen
    _lcd.setCursor(0,0);

    // prints the pages and tempo information 
    getMessage(MSG_PAGE, line);
    itoa(page, line + strlen(line),10);
    append(line, '/');
    itoa(numPages, line + strlen(line),10);
    append(line, ' ');
    itoa(tempo, line + strlen(line),10);
    append(line, ' ');
    getMessage(MSG_BPM, line + strlen(line));

    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }

    _lcd.print(line);

     // second line is empty
    _lcd.setCursor(0,1);

    line[0] = '\0';

    // prints the MIDI clock status 
    getMessage(MSG_CLOCK, line);
    isMIDIClockOn ? getMessage(ON, line + strlen (line)) : getMessage(OFF, line + strlen (line));

    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }

    _lcd.print(line);  
}

/*
* Display the default message when enter on EDIT mode
*/
void ScreenManager::printSelectComponentMessage()
{
    char line[COLUMNS+1]; 

    _lcd.noBlink();
    _lcd.setCursor(0,0);
    
    // first line
    getMessage(MSG_EDIT1, line);

    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }

    _lcd.print(line);

    // second line
    _lcd.setCursor(0,1);

    line[0] = '\0';

    getMessage(MSG_EDIT2, line);

    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }

    _lcd.print(line);            
}

/*
* Display edit global configuration screen
* GlobalConfig: information to be printed on the screen 
*/
void ScreenManager::printEditGlobalConfig(GlobalConfig globalConf)
{
    char line[COLUMNS+1]; 

    // No MIDI component is assigned to the Screen Manager
    _displayedMIDIComponent = NULL;

    _lcd.noBlink();
    _lcd.setCursor(0,0);

    // prints the musical mode
    getMessage(MSG_MODE, line);
    strcat(line + strlen(line), MIDIUtils::getModeName(globalConf.getMode()));
    
    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }

    _lcd.print(line);    

    // prints the root note and the MIDI Channel data
    _lcd.setCursor(0,1);

    line[0] = '\0';

    getMessage(MSG_KEY, line);  
    strcat(line + strlen(line), MIDIUtils::getNoteName(globalConf.getRootNote()));

    for (int i=strlen(line); i<EDIT_GLOBAL_CHANNEL_POS; i++)
    {
        append(line, ' ');
    }

    getMessage(MSG_CHANNEL, line + strlen(line));
    itoa(globalConf.getMIDIChannel(), line + strlen(line), 10);

    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }

    _lcd.print(line);

    _lcd.setCursor(EDIT_GLOBAL_MODE_POS,0);
    _lcd.blink();   
}

/*
* Load a string of chars from the PROGMEM
* msgIndex: text to Load
* buffer: array of chars where the text will be loaded
*/
void ScreenManager::getMessage(uint8_t msgIndex, char * buffer)
{
    strcpy_P(buffer, (char*)pgm_read_word(&(messages[msgIndex])));    
}

/*
* Append a char to a String
* char * s: string destination
* char c: the char to be appended
*/
void ScreenManager::append(char* s, char c) {
    
    uint8_t len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

/*
* Return the MIDI component currently assigned to the screen
*/
IMIDIComponent * ScreenManager::getDisplayedMIDIComponent()
{
    return _displayedMIDIComponent;
}

/*
* Return the index of the MIDI message currently displayed on screen
*/
uint8_t ScreenManager::getDisplayedMessageIndex()
{
    return _currentMIDIMessageDisplayed;
}

/*
* Return the type of the MIDI message currently displayed on the screen
*/
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

/*
* Display the information of a MIDI message of the MIDI component assignet to the screen.
* msgIndex: is the index of the component's MIDI message that is displayed
*/
void ScreenManager::displayComponentMIDIMessage(uint8_t msgIndex)
{
    char line[COLUMNS+1];

    if (_displayedMIDIComponent != NULL)
    {    
        // set the currently MIDI message being displayed
        _currentMIDIMessageDisplayed = msgIndex;

        _lcd.setCursor(0,0);
        
        // display current message index and total messages of the component
        itoa(msgIndex, line, DEC);
        append(line, '/');
        itoa(_displayedMIDIComponent->getNumMessages(), line + strlen(line), DEC);
        append(line, ' ');          

        // display the first MIDI message of the component
        switch((_displayedMIDIComponent->getMessages()[msgIndex-1]).getType())
        {
            case midi::NoteOn:
            case midi::NoteOff:
                getMessage(MSG_NOTE_ON_OFF, line + strlen(line));  
                _lcd.print(line);            
                printNoteOnOffMIDIData(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;

            case midi::ControlChange:
                getMessage(MSG_CTRL_CHANGE, line + strlen(line));  
                _lcd.print(line);         
                printCCMIDIData(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;

            case midi::ProgramChange:
                getMessage(MSG_PGRM_CHANGE, line + strlen(line));  
                _lcd.print(line);   
                printPCMIDIData(_displayedMIDIComponent->getMessages()[msgIndex-1]);
            break;

            case midi::InvalidType:
                getMessage(MSG_EMPTY_MIDI_TYPE, line + strlen(line));  
                _lcd.print(line);    

                 line[0] = '\0';

                _lcd.setCursor(0,1);

                for (int i=0; i<COLUMNS; i++)
                {
                    append(line + strlen(line), ' ');
                }
           
                _lcd.print(line);        
            break;          
        }

        // display cursor for editing the message getType
        _lcd.setCursor(MESSAGE_TYPE_POS,0);
        _lcd.blink();
    }    
}

/*
* Display the information of a Note On/Off MIDI message
* message: the MIDI message to display
*/
void ScreenManager::printNoteOnOffMIDIData(MIDIMessage message)
{
    char line[COLUMNS+1];
        
    _lcd.setCursor(NOTE_POS,1);

    //print note + octave + velocity data
    strcpy(line, MIDIUtils::getNoteName(message.getDataByte1()));
    itoa(MIDIUtils::getOctave(message.getDataByte1()), line + strlen(line), DEC);

    for (int i=strlen(line); i<VELOCITY_POS; i++)
    {
        append(line + strlen(line), ' ');
    }
    
    getMessage(MSG_VELOCITY, line + strlen(line));  
    itoa(message.getDataByte2(), line + strlen(line), DEC);
    
    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line + strlen(line), ' ');
    }

    _lcd.print(line);
}

/*
* Display the information of a CC MIDI message
* message: the MIDI message to display
*/
void ScreenManager::printCCMIDIData(MIDIMessage message)
{
    char line[COLUMNS+1];
   
    //print CC Number
    _lcd.setCursor(CC_POS,1);   
    getMessage(MSG_CC, line);
    itoa(message.getDataByte1(), line + strlen(line), DEC);

    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line + strlen(line), ' ');
    }

    _lcd.print(line);
}

/*
* Display the information of a Program Change MIDI message
* message: the MIDI message to display
*/
void ScreenManager::printPCMIDIData(MIDIMessage message)
{
    char line[COLUMNS+1];

    _lcd.setCursor(0,1);
    
    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line + strlen(line), ' ');
    }

    _lcd.print(line);
}

/*
* Returns true if a MIDI component has been assigned to the screen
*/
uint8_t ScreenManager::isComponentDisplayed()
{
    return _displayedMIDIComponent == NULL ? 0 : 1;  
}

/*
* Display the previous MIDI message of the component assigned to the screen
*/
void ScreenManager::displayPreviousMIDIMsg()
{
    if (_currentMIDIMessageDisplayed > 1)
    {
        _currentMIDIMessageDisplayed -= 1;
        displayComponentMIDIMessage(_currentMIDIMessageDisplayed);
    }
}

/*
* Display the next MIDI message of the component assigned to the screen
*/
void ScreenManager::displayNextMIDIMsg()
{
    if (_currentMIDIMessageDisplayed < _displayedMIDIComponent->getNumMessages())
    {
        _currentMIDIMessageDisplayed += 1;
        displayComponentMIDIMessage(_currentMIDIMessageDisplayed);
    }    
}

/*
* Assign a MIDI component to the screen to display its MIDI messages information
* midiComponent: the MIDI component to be displayed
*/ 
void ScreenManager::setMIDIComponentToDisplay(IMIDIComponent * midiComponent)
{
    _displayedMIDIComponent = midiComponent;
}

/*
* Refresh the information displayed on the screen when a parameter's value has changed
*/
void ScreenManager::refreshMIDIData()
{
    char line[COLUMNS+1];   

    switch(_displayedMIDIComponent->getMessages()[_currentMIDIMessageDisplayed-1].getType())
    {
        case midi::NoteOn:
        case midi::NoteOff:
            getMessage(MSG_NOTE_ON_OFF, line);             
        break;        

        case midi::ControlChange:
            getMessage(MSG_CTRL_CHANGE, line);                   
        break;

        case midi::ProgramChange:
            getMessage(MSG_PGRM_CHANGE, line);                   
        break;

        case midi::InvalidType:
            getMessage(MSG_EMPTY_MIDI_TYPE, line);                   
        break;
    }
    
    for (int i=(MESSAGE_TYPE_POS + strlen(line)); i<COLUMNS; i++)
    {
        append(line + strlen(line), ' ');
    }
    
    // print hte MIDI message type name on screen
    _lcd.print(line);
         
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

        case midi::InvalidType:
            
            line[0] = '\0';

            _lcd.setCursor(0,1);

            for (int i=0; i<COLUMNS; i++)
            {
                append(line + strlen(line), ' ');
            }
           
            _lcd.print(line);            
    }

    // set the cursor at the beginning of the MIDI message type name
    _lcd.setCursor(MESSAGE_TYPE_POS,0);
}

/*
* Display the default message when a page is saved into the EEPROM
*/
void ScreenManager::printSavedMessage()
{
    char line[COLUMNS+1];
    
    _lcd.noBlink();
    _lcd.setCursor(0,0);   
    
    // print the message on the first line
    getMessage(MSG_SAVED, line);  

    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line + strlen(line), ' ');
    }

    _lcd.print(line);    

    // second line is empty
    line[0] = '\0';

    _lcd.setCursor(0,1);   
    
    for (int i=0; i<COLUMNS; i++)
    {
        append(line + strlen(line), ' ');
    }

    _lcd.print(line);
}

/*
* Move the screen cursor to the start position of the MIDI message type
*/ 
void ScreenManager::moveCursorToMsgType()
{   
    _lcd.setCursor(MESSAGE_TYPE_POS,0);
}

/*
* Move the screen cursor to the start position of the note name
*/ 
void ScreenManager::moveCursorToNote()
{
    _lcd.setCursor(NOTE_POS,1); 
}

/*
* Move the screen cursor to the start position of the velocity value
*/ 
void ScreenManager::moveCursorToVelocity()
{
    char buffer[10];    

    getMessage(MSG_CC, buffer);  
    _lcd.setCursor(VELOCITY_POS + strlen(buffer) - 1,1);   
}

/*
* Move the screen cursor to the start position of the MIDI CC type 
*/ 
void ScreenManager::moveCursorToCC()
{
    char buffer[10];
    
    getMessage(MSG_CC, buffer);  
    _lcd.setCursor(strlen(buffer),1);    
}

/*
* Move the screen cursor to the start position of the root note parameter
*/ 
void ScreenManager::moveCursorToRootNote()
{
    char buffer[10];
    
    getMessage(MSG_KEY, buffer);  
    _lcd.setCursor(strlen(buffer),1);   
}

/*
* Move the screen cursor to the start position of the MIDI Channel parameter
*/ 
void ScreenManager::moveCursorToMIDIChannel()
{
   char buffer[10];
    
   getMessage(MSG_CHANNEL, buffer);  
   _lcd.setCursor(EDIT_GLOBAL_CHANNEL_POS + strlen(buffer), 1);   
}

/*
* Move the screen cursor to the start position of the musical mode parameter
*/ 
void ScreenManager::moveCursorToMode()
{
   _lcd.setCursor(EDIT_GLOBAL_MODE_POS,0);   
}

/*
* Display the new note name and octave without refreshing all the screen data
* note: note value which name and octave will be displayed.
*/
void ScreenManager::refreshNoteValue (uint8_t note)
{
    char line[COLUMNS+1];

    _lcd.noBlink();
    
    strcpy(line, MIDIUtils::getNoteName(note));
    itoa(MIDIUtils::getOctave(note), line + strlen(line), DEC);

    for (int i=strlen(line); i<VELOCITY_POS; i++)
    {
        append(line + strlen(line), ' ');
    }   

    _lcd.print(line);
    
    moveCursorToNote();

    _lcd.blink();
}

/*
* Display the new velocity value without refreshing all the screen data
* velocity: velocity value that will be displayed.
*/
void ScreenManager::refreshVelocityValue(uint8_t velocity)
{
    char line[COLUMNS+1];

    _lcd.noBlink();  
  
    itoa(velocity, line, DEC);
    
    for (int i= 0; i<2; i++)
    {
        append(line + strlen(line), ' ');
    }

    _lcd.write(line);

    moveCursorToVelocity();

    _lcd.blink();
}

/*
* Display the new CC value without refreshing all the screen data
* cc: cc value that will be displayed.
*/
void ScreenManager::refreshCCValue(uint8_t cc)
{
    char line[4];
    
    _lcd.noBlink();
    
    itoa (cc, line, DEC);
    
    for (int i= 0; i<2; i++)
    {
        append(line + strlen(line), ' ');
    }
    
    _lcd.write(line);

    moveCursorToCC();

    _lcd.blink();
}

/*
* Display the new global configuration musical mode value without refreshing all the screen data
* mode: musical mode value that will be displayed.
*/
void ScreenManager::refreshModeData(uint8_t mode)
{   
    char line[COLUMNS + 1]; 
    
    _lcd.noBlink();

    strcpy(line, MIDIUtils::getModeName(mode));
    
    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }
    
    _lcd.print(line);
   
    moveCursorToMode();

    _lcd.blink();
}

/*
* Display the new global configuration root note value without refreshing all the screen data
* rootNote: root note value that will be displayed.
*/
void ScreenManager::refreshRootNoteData(uint8_t rootNote)
{
    char line[COLUMNS + 1];

    _lcd.noBlink();
    
    strcpy(line, MIDIUtils::getNoteName(rootNote));
    
    for (int i=EDIT_GLOBAL_KEY_POS + strlen(line); i<EDIT_GLOBAL_CHANNEL_POS; i++)
    {
        append(line, ' ');
    }
    
    _lcd.print(line);
   
    moveCursorToRootNote();

    _lcd.blink();
}

/*
* Display the new global configuration MIDI channel value without refreshing all the screen data
* midiChannel: MIDI channel value that will be displayed.
*/
void ScreenManager::refreshMIDIChannelData(uint8_t midiChannel)
{
    char line[COLUMNS + 1];

    _lcd.noBlink();

    itoa(midiChannel, line, 10);
    
    for (int i= 0; i<1; i++)
    {
        append(line, ' ');
    }
    
    _lcd.write(line);

    moveCursorToMIDIChannel();

    _lcd.blink();
}

/*
* Clean a range of characters from a row of the screen 
* row: line where the chars will be deleted
* from: initial position of the range that will be deleted
* to: last position of the range that will be deleted
*/
void ScreenManager::clearRangeOnCurentLine(uint8_t row, uint8_t from, uint8_t to)
{
    _lcd.setCursor(from, row);
    for (int i=from; i<to; i++)
    {
        _lcd.print(F(" "));
    }
}

/**************************************************/
/* SEQUENCER METHODS                              */
/**************************************************/
void ScreenManager::printDefaultSequencer(uint8_t currentSequence, uint8_t totalSequences, uint16_t tempo)
{
    char line[COLUMNS+1];

    // set to the screen manager the position of the step being edited
    _currentDisplayedStep = 1;  
    
    //Set the cursor on the top left of the screen
    _lcd.setCursor(0,0);
       
    // prints the sequence number and tempo information 
    getMessage(MSG_SEQ, line);  
    itoa(currentSequence, line + strlen(line), DEC);
    append(line + strlen(line), '/');
    itoa(totalSequences, line + strlen(line), DEC);
    append(line + strlen(line), ' ');
    itoa(tempo, line + strlen(line),10);
    append(line, ' ');

    getMessage(MSG_BPM, line + strlen(line));

    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }

    _lcd.print(line);      
   
}

void ScreenManager::updateDisplayedPlaybackStep(Step step, uint8_t sequenceLength, uint8_t currentStep)
{
    char line[COLUMNS+1];
          
    // prints the step number and note value (if active) and legato symbol (if is legato)
    _lcd.setCursor(0,1);
    
    getMessage(MSG_STEP, line);  
    itoa(currentStep, line + strlen(line), DEC);
    append(line + strlen(line), '/');
    itoa(sequenceLength, line + strlen(line), DEC);
    append(line + strlen(line), ' ');
    strcat(line + strlen(line), getStepNoteValue(step));
    
    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }
    
    _lcd.print(line);   
        
}

void ScreenManager::printEditStepData(Step step, uint8_t currentStep, uint8_t sequenceLength)
{
    char line[COLUMNS+1];
    
    // set to the screen manager the position of the step being edited
    _currentDisplayedStep = currentStep;    
    
    //Set the cursor on the top left of the screen
    _lcd.setCursor(0,0);
       
    // prints the step number and note value
    getMessage(MSG_STEP, line);  
    itoa(currentStep, line + strlen(line), DEC);
    append(line + strlen(line), '/');
    itoa(sequenceLength, line + strlen(line), DEC);
    append(line + strlen(line), ' ');
    strcat(line + strlen(line), MIDIUtils::getNoteName(step.getNote()));
    itoa(MIDIUtils::getOctave(step.getNote()), line + strlen(line), DEC);
    
    for (int i=strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }
    
    _lcd.print(line);  
   
    // prints step's enabled and legato values    
    _lcd.setCursor(0,1);

    line[0] = '\0';
    
    getMessage(MSG_STEP_LEGATO, line);  
    
    step.isLegato() ? getMessage(YES, line + strlen (line)) : getMessage(NO, line + strlen (line));

    for (int i=strlen(line); i<STEP_ENABLED_POS; i++)
    {
        append(line, ' ');
    }
    
    getMessage(MSG_STEP_ENABLED, line + strlen(line));  
    
    step.isEnabled() ? getMessage(YES, line + strlen (line)) : getMessage(NO, line + strlen (line));  

    for (int i= strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }
    
    _lcd.print(line);

    // move cursor to step note value position
    _lcd.setCursor(STEP_NOTE_POS,0);
    _lcd.blink();   
}

char * ScreenManager::getStepNoteValue (Step step)
{
    char line[COLUMNS+1];

    line[0] = '\0';

    if (!step.isEnabled())
    {
        append(line, '-');
        append(line + strlen(line), '-');        
    }

    else
    {
        strcpy(line, MIDIUtils::getNoteName(step.getNote()));
        itoa(MIDIUtils::getOctave(step.getNote()), line + strlen(line), DEC);

        if (step.isLegato())
        {
            append(line + strlen(line), '_');            
        }
    }

    return line;
}

uint8_t ScreenManager::getDisplayedStepNumber()
{
    return _currentDisplayedStep;
}

void ScreenManager::moveCursorToStepNote()
{   
    _lcd.setCursor(STEP_NOTE_POS, 0); 
}

void ScreenManager::moveCursorToStepLegato()
{
    char buffer[10];   
   
    _lcd.setCursor(STEP_LEGATO_POS,1);   

}

void ScreenManager::moveCursorToStepEnabled()
{
    char buffer[10];    

    getMessage(MSG_STEP_ENABLED, buffer);  
    _lcd.setCursor(STEP_ENABLED_POS + strlen(buffer),1);  

}

void ScreenManager::refreshStepNoteValue(uint8_t note)
{
    char line[COLUMNS+1];

    _lcd.noBlink(); 
    
    strcpy(line, MIDIUtils::getNoteName(note));
    itoa(MIDIUtils::getOctave(note), line + strlen(line), DEC);
    
    for (int i= strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }   
    
    _lcd.print(line);
    
    moveCursorToStepNote();
    _lcd.blink();
}

void ScreenManager::refreshStepLegatoValue(uint8_t legato)
{
    char line[COLUMNS+1]; 

    _lcd.noBlink();    
        
    if (legato == 0)
    {
        getMessage(NO, line);
    }

    else
    {
        getMessage(YES, line);
    }

    for (int i= STEP_LEGATO_POS + strlen(line); i<STEP_ENABLED_POS; i++)
    {
        append(line, ' ');
    }  
    
    _lcd.print(line);

    moveCursorToStepLegato();
    _lcd.blink();
}

void ScreenManager::refreshStepEnabledValue(uint8_t enabled)
{
    char line[COLUMNS+1];

    _lcd.noBlink();   
  
    if (enabled == 0)
    {
        getMessage(NO, line);
    }

    else
    {
        getMessage(YES, line);
    }

    for (int i= STEP_ENABLED_POS + strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }  
       
    _lcd.print(line);
    
    moveCursorToStepEnabled();
    _lcd.blink();
}

void ScreenManager::printEditSequencerConfig (char * playbackModeName, char * stepSizeName, uint8_t midiChannel)
{
    char line[COLUMNS+1];
    
    //Set the cursor on the top left of the screen
    _lcd.setCursor(0,0);    

    // prints the sequencer playback mode
    getMessage(MSG_PLAYBACK_MODE, line);  
    strcat(line + strlen(line), playbackModeName);
    
    for (int i= strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }  
    
    _lcd.print(line);    

    // prints the step size
    _lcd.setCursor(0,1);

    line[0] = '\0';

    getMessage(MSG_STEP_SIZE, line);    
    strcat(line + strlen(line), stepSizeName);   

    for (int i= strlen(line); i<SEQUENCER_EDIT_MIDI_CHANNEL_POS; i++)
    {
        append(line, ' ');
    } 
        
    // prints the MIDI Channel  
    getMessage(MSG_CHANNEL, line + strlen(line));
    itoa(midiChannel, line + strlen(line), DEC);

    for (int i= strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    } 

    _lcd.print(line);

    _lcd.setCursor(SEQUENCER_EDIT_PLAYBACK_MODE_POS,0);
    _lcd.blink();
}

void ScreenManager::moveCursorToPlayBackMode()
{   
    _lcd.setCursor(SEQUENCER_EDIT_PLAYBACK_MODE_POS, 0);   
}

void ScreenManager::moveCursorToStepSize()
{      
    _lcd.setCursor(SEQUENCER_EDIT_STEP_SIZE_POS, 1);
}

void ScreenManager::moveCursorToSequencerMIDIChannel()
{
    char buffer[10];
    
    getMessage(MSG_CHANNEL, buffer);  
    _lcd.setCursor(SEQUENCER_EDIT_MIDI_CHANNEL_POS + strlen(buffer), 1);   
}

void ScreenManager::refreshDisplayedPlayBackMode(char * playBackMode)
{
    char line[COLUMNS+1];

    _lcd.noBlink();

    strcpy(line, playBackMode);

    for (int i= SEQUENCER_EDIT_PLAYBACK_MODE_POS + strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }   

    _lcd.print(line);    
    
    moveCursorToPlayBackMode();
    _lcd.blink();
}

void ScreenManager::refreshDisplayedStepSizeValue(char * stepSize)
{
    char line[COLUMNS+1];

    _lcd.noBlink();

    strcpy(line, stepSize);
  
    for (int i= SEQUENCER_EDIT_STEP_SIZE_POS + strlen(line); i<SEQUENCER_EDIT_MIDI_CHANNEL_POS; i++)
    {
        append(line, ' ');
    }

    _lcd.print(line);

    moveCursorToStepSize();
    _lcd.blink();
}

void ScreenManager::refreshDisplayedSequencerMidiChannel(uint8_t midiChannel)
{
    char line[COLUMNS+1];
    
    _lcd.noBlink();

    itoa(midiChannel, line, DEC);

    for (int i= SEQUENCER_EDIT_MIDI_CHANNEL_POS + strlen(line); i<COLUMNS; i++)
    {
        append(line, ' ');
    }

    _lcd.print(line);    
    
    moveCursorToSequencerMIDIChannel();
    _lcd.blink();
}
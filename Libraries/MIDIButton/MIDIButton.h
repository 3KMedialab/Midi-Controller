/* 
    MIDIButton.h - Class for define a MIDI Button
    Created by: Daniel Segura, August 21, 2017
*/
#ifndef MIDIButton_h
#define MIDIButton_h

#include "Button.h"
#include "MIDIMessage.h"

class MIDIButton : public Button
{
    public:
        MIDIButton(uint8_t pin, uint8_t puEnable, uint8_t invert, uint32_t dbTime, MIDIMessage * onPressedMessage, MIDIMessage * onReleasedMessage);
        MIDIMessage getOnPressedMessage();
        MIDIMessage getOnReleasedMessage();
     
    private:
        MIDIMessage *_onPressedMessage;
        MIDIMessage *_onReleasedMessage;       
};
#endif
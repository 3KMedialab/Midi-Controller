/* 
    MIDIPotentiometer.h - Class for define a MIDI Potentiometer
    Created by: Daniel Segura, August 24, 2017
*/
#ifndef MIDIPotentiometer_h
#define MIDIPotentiometer_h

#include "Potentiometer.h"
#include "MIDIMessage.h"

class MIDIPotentiometer : public Potentiometer
{
    public:      
        MIDIPotentiometer(uint8_t pin, uint8_t windowSize, MIDIMessage * message);
        MIDIMessage getMessage();
        MIDIMessage getShiftMessage();
        uint8_t wasChanged ();
     
    private:
        MIDIMessage *_message;         
};
#endif
/* 
    IButton.h - Interface that define button's behaviour.
    Created by: Daniel Segura, August 17, 2017
*/

#ifndef IButton_h
#define IButton_h

#include "Arduino.h"

class IButton
{
  public:
        IButton();
        virtual uint8_t read() = 0;
        virtual uint8_t isPressed() = 0;
        virtual uint8_t isReleased() = 0;
        virtual uint8_t wasPressed() = 0;
        virtual uint8_t wasReleased() = 0;
        virtual uint8_t pressedFor(uint32_t ms) = 0;
        virtual uint8_t releasedFor(uint32_t ms) = 0;
        virtual uint32_t lastChange() = 0;  
};
#endif
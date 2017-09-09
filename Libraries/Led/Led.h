#ifndef Led_h
#define Led_h

#include "Arduino.h"
#include "DigitalComponent.h"

class Led : public DigitalComponent
{
    public:
        Led(uint8_t pin, uint8_t state);
        Led(uint8_t pin);
        void setState (uint8_t state);
    
    private:             
        uint8_t _state;       
};
#endif
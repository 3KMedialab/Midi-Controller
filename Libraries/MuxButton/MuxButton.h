#ifndef MuxButton_h
#define MuxButton_h

#include "IButton.h"
#include "MuxComponent.h"

class MuxButton : public IButton, public MuxComponent
{
    public:
        MuxButton(Multiplexer * mux, uint8_t channel, uint8_t invert, uint32_t dbTime);
        uint8_t read();
        uint8_t isPressed();
        uint8_t isReleased();
        uint8_t wasPressed();
        uint8_t wasReleased();
        uint8_t pressedFor(uint32_t ms);
        uint8_t releasedFor(uint32_t ms);
        uint32_t lastChange();
    
    private:      
        uint8_t _invert;        //if 0, interpret high state as pressed, else interpret low state as pressed
        uint8_t _state;         //current button state
        uint8_t _lastState;     //previous button state
        uint8_t _changed;       //state changed since last read
        uint32_t _time;         //time of current state (all times are in ms)
        uint32_t _lastChange;   //time of last state change
        uint32_t _dbTime;       //debounce time        
};
#endif
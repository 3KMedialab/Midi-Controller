#ifndef Step_h
#define Step_h

#include "Arduino.h"

class Step
{
  public:   
    
    uint8_t getNote();
    uint8_t isEnabled();
    uint8_t isLegato();

    void setNote (uint8_t note);
    void setEnabled(uint8_t enabled);
    void setLegato(uint8_t legato);    

  private:

    uint8_t _note;
    uint8_t _enabled;
    uint8_t _legato;
  
};
#endif
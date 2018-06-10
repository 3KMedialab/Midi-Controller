#ifndef Step_h
#define Step_h

#include "Arduino.h"

class Step
{
  public:   
    
    uint8_t getNote();
    void setNote (uint8_t note);    

  private:

    uint8_t _note;
  
};
#endif
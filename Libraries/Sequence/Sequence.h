#ifndef Sequence_h
#define Sequence_h

#include "Arduino.h"
#include "Step.h"

class Sequence
{
  public:   
    Sequence();
    enum {MAX_LENGTH = 16};
    
    Step * getSteps();
    Step getStep(uint8_t position);
    uint8_t getLength();

    void setStep(Step step, uint8_t position);
    void setLength(uint8_t length);

  private:

    Step _steps[MAX_LENGTH]; 
    uint8_t _length;

   
};
#endif
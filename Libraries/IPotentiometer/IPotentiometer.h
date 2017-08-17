/* 
    IPotentiometer.h - Interface that define button's behaviour.
    Created by: Daniel Segura, August 17, 2017
*/

#ifndef IPotentiometer_h
#define IPotentiometer_h

#include "Arduino.h"

class IPotentiometer
{
  public:
        IPotentiometer();
        virtual uint16_t getValue() = 0;
		virtual uint16_t getSmoothValue() = 0;
        virtual uint8_t wasChanged() = 0;        
};
#endif
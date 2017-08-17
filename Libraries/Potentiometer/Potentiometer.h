/*
||
|| @file Potentiometer.h
|| @version 1.2
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Provide an easy way of making/using potentiometerss
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#ifndef Potentiometer_h
#define Potentiometer_h
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "IPotentiometer.h"
#include "InputComponent.h"

const uint8_t WINDOW_SIZE = 10;

class Potentiometer : public IPotentiometer, public InputComponent {
	public:
		Potentiometer(uint8_t pin);
		uint16_t getValue();
		uint16_t getSmoothValue();	
		uint8_t wasChanged();	
		
	private:
	    uint16_t _analog[WINDOW_SIZE];
		uint8_t _analogPointer;
		uint8_t _maxPointer;
		uint16_t _lastValue;
		uint16_t _value;				
};

#endif

/*
|| @changelog
|| | 1.2.1 2011-12-11 - TVHeadedRobots : Modified includes to add Arduino 1.0 compatibility 
|| | 1.2 2009-07-21 - Alexander Brevig : Changed Lisence
|| | 1.0 2009-04-13 - Alexander Brevig : Initial Release
|| #
*/
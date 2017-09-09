#include <Led.h>

Led::Led(uint8_t pin) : DigitalComponent (pin, ComponentType::OUTPUT_DIGITAL, false)
{
    _state = LOW;
    digitalWrite(pin, _state);
}

Led::Led(uint8_t pin, uint8_t state) : DigitalComponent (pin, ComponentType::OUTPUT_DIGITAL, false)
{
    _state = state;
    digitalWrite(pin, _state);
}

void Led::setState(uint8_t state)
{
    _state = state;
    digitalWrite(_pin, _state);
}




#include <MuxButton.h>

MuxButton::MuxButton(Multiplexer * mux, uint8_t channel, uint8_t invert, uint32_t dbTime) : IButton (), MuxComponent (mux, channel)
{
    _invert = invert;
    _dbTime = dbTime;

    mux->setChannel(channel);    
    _state = digitalRead(mux->getPin());
    if (_invert != 0) _state = !_state;
    _time = millis();
    _lastState = _state;
    _changed = 0;
    _lastChange = _time;
}

/*----------------------------------------------------------------------*
 * read() returns the state of the button, 1==pressed, 0==released,     *
 * does debouncing, captures and maintains times, previous states, etc. *
 *----------------------------------------------------------------------*/
uint8_t MuxButton::read(void)
{
    static uint32_t ms;
    static uint8_t pinVal;

    _mux->setChannel(_channel);
    ms = millis();
    pinVal = digitalRead(_mux->getPin());
    if (_invert != 0) pinVal = !pinVal;
    if (ms - _lastChange < _dbTime) {
        _time = ms;
        _changed = 0;
        return _state;
    }
    else {
        _lastState = _state;
        _state = pinVal;
        _time = ms;
        if (_state != _lastState)   {
            _lastChange = ms;
            _changed = 1;
        }
        else {
            _changed = 0;
        }
        return _state;
    }
}

/*----------------------------------------------------------------------*
 * isPressed() and isReleased() check the button state when it was last *
 * read, and return false (0) or true (!=0) accordingly.                *
 * These functions do not cause the button to be read.                  *
 *----------------------------------------------------------------------*/
uint8_t MuxButton::isPressed(void)
{
    return _state == 0 ? 0 : 1;
}

uint8_t MuxButton::isReleased(void)
{
    return _state == 0 ? 1 : 0;
}

/*----------------------------------------------------------------------*
 * wasPressed() and wasReleased() check the button state to see if it   *
 * changed between the last two reads and return false (0) or           *
 * true (!=0) accordingly.                                              *
 * These functions do not cause the button to be read.                  *
 *----------------------------------------------------------------------*/
uint8_t MuxButton::wasPressed(void)
{
    return _state && _changed;
}

uint8_t MuxButton::wasReleased(void)
{
    return !_state && _changed;
}

/*----------------------------------------------------------------------*
 * pressedFor(ms) and releasedFor(ms) check to see if the button is     *
 * pressed (or released), and has been in that state for the specified  *
 * time in milliseconds. Returns false (0) or true (1) accordingly.     *
 * These functions do not cause the button to be read.                  *
 *----------------------------------------------------------------------*/
uint8_t MuxButton::pressedFor(uint32_t ms)
{
    return (_state == 1 && _time - _lastChange >= ms) ? 1 : 0;
}

uint8_t MuxButton::releasedFor(uint32_t ms)
{
    return (_state == 0 && _time - _lastChange >= ms) ? 1 : 0;
}

/*----------------------------------------------------------------------*
 * lastChange() returns the time the button last changed state,         *
 * in milliseconds.                                                     *
 *----------------------------------------------------------------------*/
uint32_t MuxButton::lastChange(void)
{
    return _lastChange;
}

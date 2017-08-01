
#include "LED.h"

#include "Arduino.h"

//-----------------------------------------------------------------
// static functions
//-----------------------------------------------------------------

bool _xor(bool A, bool B)
{
    return !A != !B;
}

//-----------------------------------------------------------------
// class methods
//-----------------------------------------------------------------

LED::LED(uint8_t pin_, bool invertedOutput_)
{
    pin = pin_;
    invertedOutput = invertedOutput_;

    pinMode(pin, OUTPUT);

    _setState(OFF);
}

//-----------------------------------------------------------------
LED::~LED()
{
}

//-----------------------------------------------------------------
void LED::_setState(LEDState newState_)
{
   bool  newState_AsBool = (bool) newState_;

   state = newState_;
   uint8_t pinValue = (uint8_t) _xor(newState_AsBool, invertedOutput);
   digitalWrite(pin, pinValue);
}

//-----------------------------------------------------------------
void LED::turnOn()
{
    _setState(ON);
}
    
//-----------------------------------------------------------------
void LED::turnOff()
{
    _setState(OFF);
}

//-----------------------------------------------------------------
void LED::toggle()
{
    if(state==ON)
        _setState(OFF);
    else
        _setState(ON);
}

//-----------------------------------------------------------------
LEDState LED::getState()
{
    return state;
}

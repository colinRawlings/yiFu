#include "pushSwitch.h"

#include "Arduino.h"

//-----------------------------------------------------------------
// class methods: public
//-----------------------------------------------------------------

pushSwitch::pushSwitch(uint8_t pin_, bool pullUp_)
{
    pin = pin_;
    pullUp = pullUp_;

    if (pullUp)
        pinMode(pin, INPUT_PULLUP);
    else
        pinMode(pin, INPUT);
}

//-----------------------------------------------------------------
pushSwitch::~pushSwitch()
{
}

//-----------------------------------------------------------------
pushSwitchState pushSwitch::getState()
{
    uint8_t pushSwitchVal = digitalRead(pin);

    if ((pullUp && pushSwitchVal == 0)||(!pullUp&&pushSwitchVal==1))
        return PRESSED;
    else
        return UNPRESSED;
}
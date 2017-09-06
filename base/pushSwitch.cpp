#include "pushSwitch.h"

#include "SWdefs.h"

#include "Arduino.h"

//-----------------------------------------------------------------
// class methods: construct
//-----------------------------------------------------------------

pushSwitch::pushSwitch(uint8_t pin_, bool pullUp_)
{
    pin = pin_;
    pullUp = pullUp_;

    if (pullUp)
        pinMode(pin, INPUT_PULLUP);
    else
        pinMode(pin, INPUT);

    T_lastRead_ms = millis();
    lastState = _getCurrentState();
}

//-----------------------------------------------------------------

pushSwitch::~pushSwitch()
{
}

//-----------------------------------------------------------------
// class methods: private
//-----------------------------------------------------------------

pushSwitchState pushSwitch::_getCurrentState()
{
    uint8_t pushSwitchVal = digitalRead(pin);

    if ((pullUp && pushSwitchVal == 0) || (!pullUp && pushSwitchVal == 1))
        return PRESSED;
    else
        return UNPRESSED;
}

//-----------------------------------------------------------------
// class methods: public
//-----------------------------------------------------------------

pushSwitchState pushSwitch::getState()
{
    if ((millis() - T_lastRead_ms) < DT_SWITCH_DEBOUNCE_WAIT_MS)
        return lastState;

    T_lastRead_ms = millis();
    lastState = _getCurrentState();

    return lastState;
}
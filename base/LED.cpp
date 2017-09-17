
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

    theBlinkMode = BLINK_OFF;

    numBlinks = 0;
    dT_blinks_ms = 0;
    T0_blinks = 0;

    _setState(OFF);
}

//-----------------------------------------------------------------
LED::~LED()
{
}

//-----------------------------------------------------------------
void LED::update()
{
    if (theBlinkMode == BLINK_OFF)
        return;

    // check for bad value
    if (dT_blinks_ms == 0)
        cancelBlinking();

    //

    unsigned long T1 = millis();
    unsigned int numCycles = (T1 - T0_blinks) / dT_blinks_ms;

    //

    if ((theBlinkMode == BLINK_FOR_N_BLINKS) && (numCycles > 2 * numBlinks))
        cancelBlinking();

    if ((numCycles % 2) == 0)
        _setState(OFF);
    else
        _setState(ON);
}

//-----------------------------------------------------------------
// class methods: state
//-----------------------------------------------------------------
void LED::_setState(LEDState newState_)
{
    bool newState_AsBool = (bool)newState_;

    uint8_t pinValue = (uint8_t)_xor(newState_AsBool, invertedOutput);
    digitalWrite(pin, pinValue);
}

//-----------------------------------------------------------------
void LED::turnOn()
{
    state = ON;

    if (theBlinkMode == BLINK_OFF)
        _setState(ON);
}

//-----------------------------------------------------------------
void LED::turnOff()
{
    state = OFF;

    if (theBlinkMode == BLINK_OFF)
        _setState(OFF);
}

//-----------------------------------------------------------------
void LED::toggle()
{
    if (state == ON)
        turnOn();
    else
        turnOff();
}

//-----------------------------------------------------------------
LEDState LED::getState()
{
    return state;
}

//-----------------------------------------------------------------
// class methods: blink
//-----------------------------------------------------------------

void LED::blink(unsigned int numBlinks_, unsigned int dT_blinks_ms_)
{
    theBlinkMode = BLINK_FOR_N_BLINKS;

    numBlinks = numBlinks_;
    dT_blinks_ms = dT_blinks_ms_;

    T0_blinks = millis();
}

//-----------------------------------------------------------------
void LED::blinkForever(unsigned int dT_blinks_ms_)
{
    theBlinkMode = BLINK_FOREVER;

    numBlinks = 0;
    dT_blinks_ms = dT_blinks_ms_;

    T0_blinks = millis();
}

//-----------------------------------------------------------------
void LED::cancelBlinking()
{
    theBlinkMode = BLINK_OFF;

    numBlinks = 0;
    dT_blinks_ms = 0;

    T0_blinks = 0;

    _setState(state);
}
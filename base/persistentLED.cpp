#include "persistentLED.h"

//-----------------------------------------------------------------
// Class definition
//-----------------------------------------------------------------

persistentLED::persistentLED(uint8_t pin_, bool invertedOutput_, long activatedDuration_us_)
    : LED(pin_, invertedOutput_)
{
    activatedDuration_us = activatedDuration_us_;
}

//-----------------------------------------------------------------
void persistentLED::setActivatedDuration(unsigned long activatedDuration_us_)
{
    activatedDuration_us = activatedDuration_us_;
}

//-----------------------------------------------------------------
void persistentLED::activate()
{
    timeWhenActivated_us = micros();
    turnOn();
}

//-----------------------------------------------------------------
void persistentLED::update()
{
    if (getState() == OFF)
        return;

    if ((micros() - timeWhenActivated_us) > activatedDuration_us)
        turnOff();
}
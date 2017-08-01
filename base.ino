#include "HWdefs.h"
#include "persistentLED.h"
#include "pushSwitch.h"

#include "Arduino.h"

persistentLED *errorLED = new persistentLED(LED_PIN, false,2e6);
pushSwitch *theInput = new pushSwitch(SWITCH_PIN, true);

void setup()
{
}

void loop()
{
    if(theInput->getState()==PRESSED)
        errorLED->activate();

    performUpdates();
}

void performUpdates()
{
    errorLED->update();
}
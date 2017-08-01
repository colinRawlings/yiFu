#ifndef _PUSH_SWITCH_H
#define _PUSH_SWITCH_H

#include "Arduino.h"

enum pushSwitchState : uint8_t
{
    UNPRESSED = 0,
    PRESSED
};

class pushSwitch
{
  private:
    uint8_t pin;
    bool pullUp;

  public:

    pushSwitch(uint8_t pin_, bool pullUp_);
    ~pushSwitch();

    pushSwitchState getState();
};

#endif //_PUSH_SWITCH_H
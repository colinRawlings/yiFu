// base class for the LED's

#ifndef _LED_H
#define _LED_H

#include "Arduino.h"

enum LEDState : uint8_t
{
  OFF = 0,
  ON
};

class LED
{
private:
  uint8_t pin;
  LEDState state;
  bool invertedOutput;

private:
  void _setState(LEDState newState);

public:
  LED(uint8_t pin_, bool invertedOutput_);
  ~LED();

  void turnOn();
  void turnOff();
  void toggle();

  LEDState getState();
};

#endif //_LED_H

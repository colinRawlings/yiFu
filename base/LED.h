// base class for the LED's
//
// - blink requests take precedence over state until they complete
// - A new blink request cancels an existing blink request

#ifndef _LED_H
#define _LED_H

#include "Arduino.h"

//-----------------------------------------------------------------
// types
//-----------------------------------------------------------------

enum LEDState : uint8_t
{
  OFF = 0,
  ON
};

//-----------------------------------------------------------------
enum BlinkMode
{
  BLINK_OFF,
  BLINK_FOR_N_BLINKS,
  BLINK_FOREVER
};

//-----------------------------------------------------------------
// class
//-----------------------------------------------------------------

class LED
{
private:
  uint8_t pin;
  LEDState state;
  bool invertedOutput;

  BlinkMode theBlinkMode;
  unsigned int numBlinks;
  unsigned int dT_blinks_ms;
  unsigned long T0_blinks;

private:
  void _setState(LEDState newState);

public:
  LED(uint8_t pin_, bool invertedOutput_);
  ~LED();

  void turnOn();
  void turnOff();
  void toggle();
  LEDState getState();

  void blink(unsigned int numBlinks_, unsigned int dT_blinks_ms_);
  void blinkForever(unsigned int dT_blinks_ms_);
  void cancelBlinking();

  void update();
};

#endif //_LED_H

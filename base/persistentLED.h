#ifndef _PERSISTENT_LED_H
#define _PERSISTENT_LED_H

/* 
persistent LED, provides the ability to hold an ON state for
 a fixed interval
*/

#include "Arduino.h"
#include "LED.h"

class persistentLED : public LED
{
private:
  unsigned long timeWhenActivated_us;
  unsigned long activatedDuration_us;

public:
  persistentLED(uint8_t pin_, bool invertedOutput_, long activatedDuration_us_);

  void setActivatedDuration(unsigned long activatedDuration_us_);
  void update();
  void activate();
};

#endif // _PERSISTENT_LED_H
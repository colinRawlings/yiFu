#ifndef _LENS_PORT_INTERFACE_H
#define _LENS_PORT_INTERFACE_H

#include "errors.h"
#include "Arduino.h"

enum msgSpeed
{
  SLOW = 0,
  FAST
};

class lensPortInterface
{
public:
  virtual errorCodes getMsgLength(unsigned int &msgLength_) = 0;
  virtual errorCodes getMsgSpeed(msgSpeed &theSpeed) = 0;
  virtual errorCodes getMsg(uint8_t msg[], unsigned int &msgLength_) = 0;
  virtual errorCodes getAnswer(uint8_t answer[], unsigned int &answerLength_) = 0;
};

#endif // _LENS_PORT_INTERFACE_H
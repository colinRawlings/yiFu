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
  virtual ErrorCode setMsg(uint8_t msg[], unsigned int msgLength_) = 0;

  virtual ErrorCode sendFastMsg() = 0;
  virtual ErrorCode sendSlowMsg() = 0;

  virtual ErrorCode getMsgLength(unsigned int &msgLength_) = 0;
  virtual ErrorCode getMsgSpeed(msgSpeed &theSpeed) = 0;
  virtual ErrorCode getMsg(uint8_t msg[], unsigned int &msgLength_) = 0;
  virtual ErrorCode getAnswer(uint8_t answer[], unsigned int &answerLength_) = 0;
};

#endif // _LENS_PORT_INTERFACE_H
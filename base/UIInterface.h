#ifndef _UIInterface_H
#define _UIInterface_H

#include "Arduino.h"
#include "errors.h"

class lensManagerInterface;

class UIInterface
{
public:
  virtual void setLensManager(lensManagerInterface *theLens_) = 0;
  virtual void setSerialPort(Stream *theSerialPort_) = 0;

  virtual void initLens() = 0;

  virtual void update() = 0;
};

#endif // _UI_INTERFACE_H
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

  virtual void reportError(errorCodes theErrorCode);
  virtual void displayStartupError();
  virtual void displayEndStop();

  virtual void displayNotReady();
  virtual void displayReady();

  virtual errorCodes displayLensConversation() = 0;
  virtual void reportFocalLengths() = 0;

  virtual void update();
};

#endif // _UI_INTERFACE_H
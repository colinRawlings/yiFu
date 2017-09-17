#ifndef _LENS_INITIALIZER_H
#define _LENS_INITIALIZER_H

#include "lensPortInterface.h"

#include "lensInitializerInterface.h"

#include "errors.h"
#include "Arduino.h"

class lensPortInterface;

class lensInitializer : public lensInitializerInterface
{
private:
  lensPortInterface *the_lens_port;
  int FLmin_mm; // surrogate for lens ID
  int FLmax_mm;

private:
  void _initPower();
  void _turnOnPower();
  void _turnOffPower();

  ErrorCode _sendCR(msgSpeed speed);
  ErrorCode _activateISControl();
  ErrorCode _activateTelePhotoMFControl();
  ErrorCode _activatePrimeMFControl();
  ErrorCode _activateLensControls();
  ErrorCode _activateLens();

public:
  lensInitializer();

  void setTheLensPort(lensPortInterface *the_lens_port_);

  ErrorCode initLens();
  ErrorCode resetLens();

  void getLensFocalLengths(int &minFocalLength_mm, int &maxFocalLength_mm);
};

#endif // _LENS_INITIALIZER_H
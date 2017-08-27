// overseas the activities of the lens, interacts with the UI

#ifndef _LENS_MANAGER_H
#define _LENS_MANAGER_H

//
#include "lensManagerInterface.h"

#include "lensPort.h"

#include "errors.h"
#include "Arduino.h"

//
class UIInterface;
class lensPortInterface;

class lensManager : public lensManagerInterface
{
private:
  UIInterface *the_ui;
  lensPort the_lens_port;

  int FD_plus;
  int FD_minus;

  int FLmin_mm; // surrogate for lens ID
  int FLmax_mm;

private:
  void _initPower();
  void _turnOnPower();
  void _turnOffPower();

  errorCodes _sendCR(msgSpeed speed);
  errorCodes _activateISControl();
  errorCodes _activateTelePhotoMFControl();
  errorCodes _activatePrimeMFControl();
  errorCodes _activateLensControls();
  errorCodes _activateLens();

public:
  // byte conversion functions

public:
  lensManager();
  void setUI(UIInterface *ui_);
  errorCodes initLens();

  void getLensFocalLengths(int &minFocalLength_mm, int &maxFocalLength_mm);
  lensPortInterface *getLensPort();
};

#endif // _LENS_MANAGER_H
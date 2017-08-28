#ifndef _LENS_MANAGER_INTERFACE_H
#define _LENS_MANAGER_INTERFACE_H

#include "errors.h"
#include "Arduino.h"

class UIInterface;
class lensPortInterface;
class focalDistanceManagerInterface;

class lensManagerInterface
{
public:
  virtual void setUI(UIInterface *the_ui_) = 0;
  virtual errorCodes initLens() = 0;

  virtual void getLensFocalLengths(int &minFocalLength_mm, int &maxFocalLength_mm) = 0;
  virtual lensPortInterface *getLensPort() = 0;
  virtual focalDistanceManagerInterface *getFocalDistanceManager() = 0;
};

#endif //_LENS_MANAGER_INTERFACE_H
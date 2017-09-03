#ifndef _APERTURE_MANAGER_INTERFACE_H
#define _APERTURE_MANAGER_INTERFACE_H

#include "errors.h"

class apertureManagerInterface
{
public:
  virtual errorCodes openFully() = 0;
  virtual errorCodes openOneStep() = 0;
  virtual errorCodes closeOneStep() = 0;
};

#endif //_APERTURE_MANAGER_INTERFACE_H
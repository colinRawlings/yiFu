#ifndef _APERTURE_MANAGER_INTERFACE_H
#define _APERTURE_MANAGER_INTERFACE_H

#include "errors.h"

class apertureManagerInterface
{
public:
  virtual errorCode openFully() = 0;
  virtual errorCode openOneStep() = 0;
  virtual errorCode closeOneStep() = 0;

  virtual errorCode getApertureValue_tics(unsigned int &av_tics) = 0;
};

#endif //_APERTURE_MANAGER_INTERFACE_H
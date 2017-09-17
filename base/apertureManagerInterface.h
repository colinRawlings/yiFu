#ifndef _APERTURE_MANAGER_INTERFACE_H
#define _APERTURE_MANAGER_INTERFACE_H

#include "errors.h"

class apertureManagerInterface
{
public:
  virtual ErrorCode openFully() = 0;
  virtual ErrorCode openOneStep() = 0;
  virtual ErrorCode closeOneStep() = 0;

  virtual ErrorCode getApertureValue_tics(unsigned int &av_tics) = 0;
};

#endif //_APERTURE_MANAGER_INTERFACE_H
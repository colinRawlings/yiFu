#ifndef _FOCAL_LENGTH_MANAGER_INTERFACE_H
#define _FOCAL_LENGTH_MANAGER_INTERFACE_H

#include "errors.h"

class focalLengthManagerInterface
{
public:
  virtual ErrorCode getFocalLength(int &focalLength) = 0;
};

#endif //_FOCAL_LENGTH_MANAGER_INTERFACE_H
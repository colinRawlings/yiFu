#ifndef _FOCAL_DISTANCE_MANAGER_INTERFACE_H
#define _FOCAL_DISTANCE_MANAGER_INTERFACE_H

#include "errors.h"

class focalDistanceManagerInterface
{
public:
  virtual ErrorCode gotoMinFocalDistance() = 0;
  virtual ErrorCode gotoInfFocalDistance() = 0;

  virtual void setFocalDistanceMemoryPlus(int focalDistance) = 0;
  virtual void setFocalDistanceMemoryMinus(int focalDistance) = 0;

  virtual ErrorCode gotoFocalDistancePlus() = 0;
  virtual ErrorCode gotoFocalDistanceMinus() = 0;

  virtual ErrorCode stepFocalDistance(int step) = 0;
  virtual ErrorCode setFocalDistance(int focalDistance) = 0;

  virtual ErrorCode getFocalDistance(int &focalDistance) = 0;
};

#endif //_FOCAL_DISTANCE_MANAGER_INTERFACE_H

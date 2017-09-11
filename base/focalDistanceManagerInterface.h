#ifndef _FOCAL_DISTANCE_MANAGER_INTERFACE_H
#define _FOCAL_DISTANCE_MANAGER_INTERFACE_H

#include "errors.h"

class focalDistanceManagerInterface
{
public:
  virtual errorCode gotoMinFocalDistance() = 0;
  virtual errorCode gotoInfFocalDistance() = 0;

  virtual void setFocalDistanceMemoryPlus(int focalDistance) = 0;
  virtual void setFocalDistanceMemoryMinus(int focalDistance) = 0;

  virtual errorCode gotoFocalDistancePlus() = 0;
  virtual errorCode gotoFocalDistanceMinus() = 0;

  virtual errorCode stepFocalDistance(int step) = 0;
  virtual errorCode setFocalDistance(int focalDistance) = 0;

  virtual errorCode getFocalDistance(int &focalDistance) = 0;
};

#endif //_FOCAL_DISTANCE_MANAGER_INTERFACE_H

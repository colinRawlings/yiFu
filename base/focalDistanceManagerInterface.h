#ifndef _FOCAL_DISTANCE_MANAGER_INTERFACE_H
#define _FOCAL_DISTANCE_MANAGER_INTERFACE_H

#include "errors.h"

class focalDistanceManagerInterface
{
  public:
    virtual errorCodes gotoMinFocalDistance() = 0;
    virtual errorCodes gotoInfFocalDistance() = 0;

    virtual void setFocalDistanceMemoryPlus(int focalDistance) = 0;
    virtual void setFocalDistanceMemoryMinus(int focalDistance) = 0;

    virtual errorCodes gotoFocalDistancePlus() = 0;
    virtual errorCodes gotoFocalDistanceMinus() = 0;

    virtual errorCodes stepFocalDistance(int step) = 0;
    virtual errorCodes setFocalDistance(int focalDistance) = 0;

    virtual errorCodes getFocalDistance(int &focalDistance) = 0;
};

#endif //_FOCAL_DISTANCE_MANAGER_INTERFACE_H

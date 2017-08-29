#ifndef _FOCAL_DISTANCE_MANAGER_H
#define _FOCAL_DISTANCE_MANAGER_H

#include "focalDistanceManagerInterface.h"

class lensPortInterface;

#include "Arduino.h"

//-----------------------------------------------------------------
struct focusStatus
{
    bool autoFocusMode;
    bool focusMotorMoving;
    bool focusMotorAccelerating;
    bool focusMotorAtEndStop;
};

//-----------------------------------------------------------------
class focalDistanceManager : public focalDistanceManagerInterface
{
  private:
    int focalDistanceMemoryPlus;
    bool focalDistanceMemoryPlusSet;

    int focalDistanceMemoryMinus;
    bool focalDistanceMemoryMinusSet;

    lensPortInterface *the_lens_port;

  private:
    errorCodes _waitMoveComplete();
    errorCodes _getStatus(focusStatus &status);

  public:
    focalDistanceManager();

    void setTheLensPort(lensPortInterface *the_lens_port_);

    errorCodes gotoMinFocalDistance();
    errorCodes gotoInfFocalDistance();

    void setFocalDistanceMemoryPlus(int focalDistance);
    void setFocalDistanceMemoryMinus(int focalDistance);

    errorCodes gotoFocalDistancePlus();
    errorCodes gotoFocalDistanceMinus();

    errorCodes stepFocalDistance(int step);
    errorCodes setFocalDistance(int focalDistance);

    errorCodes getFocalDistance(int &focalDistance);
};

#endif // _FOCAL_DISTANCE_MANAGER_H
#ifndef _FOCAL_DISTANCE_MANAGER_H
#define _FOCAL_DISTANCE_MANAGER_H

#include "focalDistanceManagerInterface.h"

#include "errors.h"
#include "Arduino.h"

class lensPortInterface;

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
  errorCode _waitMoveComplete();
  errorCode _getStatus(focusStatus &status);

public:
  focalDistanceManager();

  void setTheLensPort(lensPortInterface *the_lens_port_);

  errorCode gotoMinFocalDistance();
  errorCode gotoInfFocalDistance();

  void setFocalDistanceMemoryPlus(int focalDistance);
  void setFocalDistanceMemoryMinus(int focalDistance);

  errorCode gotoFocalDistancePlus();
  errorCode gotoFocalDistanceMinus();

  errorCode stepFocalDistance(int step);
  errorCode setFocalDistance(int focalDistance);

  errorCode getFocalDistance(int &focalDistance);
};

#endif // _FOCAL_DISTANCE_MANAGER_H
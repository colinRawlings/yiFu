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
  ErrorCode _waitMoveComplete();
  ErrorCode _getStatus(focusStatus &status);

public:
  focalDistanceManager();

  void setTheLensPort(lensPortInterface *the_lens_port_);

  ErrorCode gotoMinFocalDistance();
  ErrorCode gotoInfFocalDistance();

  void setFocalDistanceMemoryPlus(int focalDistance);
  void setFocalDistanceMemoryMinus(int focalDistance);

  ErrorCode gotoFocalDistancePlus();
  ErrorCode gotoFocalDistanceMinus();

  ErrorCode stepFocalDistance(int step);
  ErrorCode setFocalDistance(int focalDistance);

  ErrorCode getFocalDistance(int &focalDistance);
};

#endif // _FOCAL_DISTANCE_MANAGER_H
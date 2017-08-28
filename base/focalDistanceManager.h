#ifndef _FOCAL_DISTANCE_MANAGER_H
#define _FOCAL_DISTANCE_MANAGER_H

#include "focalDistanceManagerInterface.h"

class lensPortInterface;

#include "Arduino.h"

class focalDistanceManager : public focalDistanceManagerInterface
{
  private:
    int focalDistanceMemoryPlus;
    bool focalDistanceMemoryPlusSet;

    int focalDistanceMemoryMinus;
    bool focalDistanceMemoryMinusSet;

    lensPortInterface *the_lens_port;

  public:
    focalDistanceManager();

    void setTheLensPort(lensPortInterface *the_lens_port_);

    errorCodes gotoMinFocalDistance();
    errorCodes gotoInfFocalDistance();
    errorCodes getFocalDistance(int &focalDistance);
};

#endif // _FOCAL_DISTANCE_MANAGER_H
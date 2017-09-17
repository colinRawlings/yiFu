#ifndef _FOCAL_LENGTH_MANAGER_H
#define _FOCAL_LENGTH_MANAGER_H

#include "focalLengthManagerInterface.h"

class lensPortInterface;

class focalLengthManager : public focalLengthManagerInterface
{
private:
  lensPortInterface *the_lens_port;

public:
  focalLengthManager();

  void setTheLensPort(lensPortInterface *the_lens_port);
  ErrorCode getFocalLength(int &focalLength);
};

#endif //_FOCAL_LENGTH_MANAGER_H
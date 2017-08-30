#ifndef _LENS_MANAGER_INTERFACE_H
#define _LENS_MANAGER_INTERFACE_H

#include "errors.h"
#include "Arduino.h"

class lensPortInterface;

class focalDistanceManagerInterface;
class lensInitializerInterface;

class lensManagerInterface
{
public:
  virtual lensPortInterface *getLensPort() = 0;

  virtual focalDistanceManagerInterface *getFocalDistanceManager() = 0;
  virtual lensInitializerInterface *getLensInitializer() = 0;
};

#endif //_LENS_MANAGER_INTERFACE_H
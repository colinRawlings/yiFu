// overseas the activities of the lens, interacts with the UI

#ifndef _LENS_MANAGER_H
#define _LENS_MANAGER_H

//
#include "lensManagerInterface.h"

#include "lensPort.h"
#include "focalDistanceManager.h"
#include "lensInitializer.h"

#include "errors.h"
#include "Arduino.h"

//
class UIInterface;
class lensPortInterface;
class lensInitializerInterface;

class lensManager : public lensManagerInterface
{
private:
  lensPort the_lens_port;

  focalDistanceManager the_fd_manager;
  lensInitializer the_lens_initializer;

public:
  lensManager();

  lensPortInterface *getLensPort();

  focalDistanceManagerInterface *getFocalDistanceManager();
  lensInitializerInterface *getLensInitializer();
};

#endif // _LENS_MANAGER_H
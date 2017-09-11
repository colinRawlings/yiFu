// overseas the activities of the lens, interacts with the UI

#ifndef _LENS_MANAGER_H
#define _LENS_MANAGER_H

//
#include "lensManagerInterface.h"

#include "lensPort.h"
#include "lensInitializer.h"
#include "focalDistanceManager.h"
#include "apertureManager.h"

#include "errors.h"
#include "Arduino.h"

//

class UIInterface;
class lensPortInterface;
class lensInitializerInterface;
class aperertureManagerInterface;

//

class lensManager : public lensManagerInterface
{
private:
  lensPort the_lens_port;

  focalDistanceManager the_fd_manager;
  lensInitializer the_lens_initializer;
  apertureManager the_av_manager;

public:
  lensManager();

  lensInitializerInterface *getLensInitializer();
  focalDistanceManagerInterface *getFocalDistanceManager();
  apertureManagerInterface *getApertureManager();

  errorCode sendLensCommand(lensCommand cmd);
  errorCode getLensConversation(lensConversation &conv);
};

#endif // _LENS_MANAGER_H
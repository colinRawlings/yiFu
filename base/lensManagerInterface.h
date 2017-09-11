#ifndef _LENS_MANAGER_INTERFACE_H
#define _LENS_MANAGER_INTERFACE_H

#include "SWdefs.h"

#include "lensPortInterface.h" // for msgSpeed defn

#include "errors.h"
#include "Arduino.h"

class lensInitializerInterface;
class focalDistanceManagerInterface;
class apertureManagerInterface;

//-----------------------------------------------------------------
// structs
//-----------------------------------------------------------------
struct lensCommand
{
  byte msg[LENS_BUFFER_LENGTH];
  unsigned int msgLength;
  msgSpeed speed;
};

//-----------------------------------------------------------------
struct lensConversation
{
  byte msg[LENS_BUFFER_LENGTH];
  byte answer[LENS_BUFFER_LENGTH];

  unsigned int msgLength;
  bool answerAvailable;
  msgSpeed speed;
};

//-----------------------------------------------------------------
// Class
//-----------------------------------------------------------------
class lensManagerInterface
{
public:
  virtual lensInitializerInterface *getLensInitializer() = 0;
  virtual focalDistanceManagerInterface *getFocalDistanceManager() = 0;
  virtual apertureManagerInterface *getApertureManager() = 0;

  virtual errorCode sendLensCommand(lensCommand cmd) = 0;
  virtual errorCode getLensConversation(lensConversation &conv) = 0;
};

#endif //_LENS_MANAGER_INTERFACE_H
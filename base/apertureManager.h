#ifndef _APERTURE_MANAGER_H
#define _APERTURE_MANAGER_H

#include "apertureManagerInterface.h"

#include "errors.h"

class lensPortInterface;
class lensInitializerInterface;

enum apertureCommand
{
  OPEN_FULLY = 0,
  OPEN_ONE_STEP,
  CLOSE_ONE_STEP
};

class apertureManager : public apertureManagerInterface
{

private:
  lensPortInterface *the_lens_port;
  lensInitializerInterface *the_lens_initializer;

  unsigned int apertureValue_tics;
  bool apertureValueKnown;

private:
  ErrorCode _executeApertureCommand(apertureCommand cmd);

public:
  apertureManager();
  void setTheLensPort(lensPortInterface *the_lens_port_);
  void setTheLensInitializer(lensInitializerInterface *the_lens_initializer_);

  ErrorCode openFully();
  ErrorCode openOneStep();
  ErrorCode closeOneStep();

  ErrorCode getApertureValue_tics(unsigned int &av_tics);
};

#endif //_APERTURE_MANAGER_H
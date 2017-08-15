// base class for the parts of the lens

#ifndef _LENS_COMPONENT_H
#define _LENS_COMPONENT_H

#include "Arduino.h"
#include "HWdefs.h"
#include "lensInterface.h"
#include "UI.h"

class lensComponent
{
private:


protected:
  lensInterface *theLens;
  bool verboseMode:

protected:


public:
  lensComponent(lensInterface *theLens_, UI *theUI_);

};

#endif
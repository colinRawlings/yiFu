#ifndef _LENS_INITIALIZER_INTERFACE_H
#define _LENS_INITIALIZER_INTERFACE_H

#include "errors.h"
#include "Arduino.h"

class lensPortInterface;

class lensInitializerInterface
{

  public:
    virtual void setTheLensPort(lensPortInterface *the_lens_port_) = 0;

    virtual errorCodes initLens() = 0;

    virtual errorCodes resetLens() = 0;

    virtual void getLensFocalLengths(int &minFocalLength_mm, int &maxFocalLength_mm) = 0;
};

#endif // _LENS_INITIALIZER_INTERFACE_H
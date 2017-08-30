#ifndef _LENS_INITIALIZER_H
#define _LENS_INITIALIZER_H

#include "lensPortInterface.h"

#include "lensInitializerInterface.h"

#include "errors.h"
#include "Arduino.h"

class lensPortInterface;

class lensInitializer : public lensInitializerInterface
{
  private:
    lensPortInterface *the_lens_port;
    int FLmin_mm; // surrogate for lens ID
    int FLmax_mm;

  private:
    void _initPower();
    errorCodes _sendCR(msgSpeed speed);
    errorCodes _activateISControl();
    errorCodes _activateTelePhotoMFControl();
    errorCodes _activatePrimeMFControl();
    errorCodes _activateLensControls();
    errorCodes _activateLens();

  public:
    lensInitializer();

    void setTheLensPort(lensPortInterface *the_lens_port_);

    errorCodes initLens();

    void turnOnPower();
    void turnOffPower();
    void getLensFocalLengths(int &minFocalLength_mm, int &maxFocalLength_mm);
};

#endif // _LENS_INITIALIZER_H
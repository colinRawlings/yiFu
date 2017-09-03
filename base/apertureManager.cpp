#include "apertureManager.h"

#include "lensPortInterface.h"
#include "lensInitializerInterface.h"

#include "SWdefs.h"

#include "errors.h"

//-----------------------------------------------------------------
// Class Definition: Construct
//-----------------------------------------------------------------

apertureManager::apertureManager()
{
    apertureValueKnown = false;
    apertureValue_tics = 0;

    the_lens_initializer = NULL;
    the_lens_port = NULL;
}

//-----------------------------------------------------------------
void apertureManager::setTheLensPort(lensPortInterface *the_lens_port_)
{
    the_lens_port = the_lens_port_;
}

//-----------------------------------------------------------------
void apertureManager::setTheLensInitializer(lensInitializerInterface *the_lens_initializer_)
{
    the_lens_initializer = the_lens_initializer_;
}

//-----------------------------------------------------------------
// Class Definition: Public
//-----------------------------------------------------------------

errorCodes apertureManager::openFully()
{
    if (errorCodes err = _executeApertureCommand(OPEN_FULLY))
        return err;

    //

    apertureValue_tics = 0;
    apertureValueKnown = true;
}

//-----------------------------------------------------------------
errorCodes apertureManager::openOneStep()
{
    if (errorCodes err = _executeApertureCommand(OPEN_ONE_STEP))
        return err;

    if (apertureValueKnown)
    {
        apertureValue_tics += APERTURE_STEP_SIZE_TICS;
        if (apertureValue_tics > MAX_APERTURE_TICS)
            apertureValue_tics = APERTURE_STEP_SIZE_TICS;
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes apertureManager::closeOneStep()
{
    if (errorCodes err = _executeApertureCommand(CLOSE_ONE_STEP))
        return err;

    if (apertureValueKnown)
    {
        apertureValue_tics += APERTURE_STEP_SIZE_TICS;
        if (apertureValue_tics > MAX_APERTURE_TICS)
            apertureValue_tics = APERTURE_STEP_SIZE_TICS;
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes apertureManager::_executeApertureCommand(apertureCommand cmd)
{

    errorCodes err;

    if (the_lens_initializer == NULL)
        return AV_LENS_INITIALIZER_UNSET;

    if (the_lens_port == NULL)
        return AV_LENS_PORT_UNSET;

    //

    Serial.print("here I would try to change the aperture");

    //

    if (err = the_lens_initializer->resetLens())
        return err;
}
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
// Class Definition: Private
//-----------------------------------------------------------------

errorCode apertureManager::_executeApertureCommand(apertureCommand cmd)
{

    errorCode err;
    errorCode err2;

    if (the_lens_initializer == NULL)
        return AV_LENS_INITIALIZER_UNSET;

    if (the_lens_port == NULL)
        return AV_LENS_PORT_UNSET;

    //

    unsigned int msgLength = 4;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    if (cmd == OPEN_ONE_STEP)
    {
        msg[0] = 0x13;
        msg[1] = 0x03;
        msg[2] = 0x13;
        msg[3] = 0x03;
    }
    else if (cmd == CLOSE_ONE_STEP)
    {
        msg[0] = 0x13;
        msg[1] = 0xFD;
        msg[2] = 0x13;
        msg[3] = 0xFD;
    }
    else if (cmd == OPEN_FULLY)
    {
        Serial.println("TODO OPEN_FULLY");
        return SUCCESS;
    }

    if (err = the_lens_port->setMsg(msg, msgLength))
        return err;

    err = the_lens_port->sendSlowMsg();
    err2 = the_lens_initializer->resetLens();

    //

    if (err != SUCCESS)
        return err;

    if (err2 != SUCCESS)
        return err2;

    //

    return SUCCESS;
}

//-----------------------------------------------------------------
// Class Definition: Public
//-----------------------------------------------------------------

errorCode apertureManager::openFully()
{
    if (errorCode err = _executeApertureCommand(OPEN_FULLY))
        return err;

    //

    apertureValue_tics = 0;
    apertureValueKnown = true;
}

//-----------------------------------------------------------------
errorCode apertureManager::openOneStep()
{
    if (errorCode err = _executeApertureCommand(OPEN_ONE_STEP))
        return err;

    if (apertureValueKnown)
    {
        apertureValue_tics += APERTURE_STEP_SIZE_TICS;
        if (apertureValue_tics > MAX_APERTURE_TICS)
            apertureValue_tics = MAX_APERTURE_TICS;
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCode apertureManager::closeOneStep()
{
    if (errorCode err = _executeApertureCommand(CLOSE_ONE_STEP))
        return err;

    if (apertureValueKnown)
    {
        apertureValue_tics -= APERTURE_STEP_SIZE_TICS;
        if (apertureValue_tics < 0)
            apertureValue_tics = 0;
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCode apertureManager::getApertureValue_tics(unsigned int &av_tics)
{
    av_tics = 0;

    if (!apertureValueKnown)
        return AV_AV_UNKNOWN;

    av_tics = apertureValue_tics;
}
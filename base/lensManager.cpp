#include "lensManager.h"

#include "UIInterface.h"
#include "lensPortInterface.h"

#include "errors.h"
#include "Arduino.h"

#include "HWdefs.h"
#include "SWdefs.h"

//-----------------------------------------------------------------
// Class Definition: Construct
//-----------------------------------------------------------------
lensManager::lensManager()
    : the_lens_port()
{
    _initPower();
    // set ref to the_lens_port
}

//-----------------------------------------------------------------
void lensManager::setUI(UIInterface *the_ui_)
{
    the_ui = the_ui_;
}

//-----------------------------------------------------------------
void lensManager::_initPower()
{
    pinMode(lensPowerPin, OUTPUT);
}

//-----------------------------------------------------------------
void lensManager::_turnOnPower()
{
    digitalWrite(lensPowerPin, LPOW_ON);
}

//-----------------------------------------------------------------
void lensManager::_turnOffPower()
{
    digitalWrite(lensPowerPin, LPOW_OFF);
}

//-----------------------------------------------------------------
errorCodes lensManager::_sendCR(bool fastMode)
{
    unsigned int CRLength = 3;
    uint8_t CRmsg[] = {0x00, 0x0A, 0x00};
    uint8_t CRAnswer[CRLength];
    unsigned int answerLength = 0;

    //

    if (errorCodes err = the_lens_port.setMsg(CRmsg, CRLength))
        return err;

    if (fastMode)
    {
        if (errorCodes err = the_lens_port.sendFastMsg())
            return err;
    }
    else
    {
        if (errorCodes err = the_lens_port.sendSlowMsg())
            return err;
    }

    //

    if (errorCodes err = the_lens_port.getAnswer(CRAnswer, answerLength))
        return err;

    if (answerLength != CRLength)
        return LENS_MAN_BAD_CR_LENGTH;

    if (CRAnswer[2] != 0xAA)
        return LENS_MAN_BAD_CR_RESPONSE;

    return SUCCESS;
}

//-----------------------------------------------------------------
// public methods
//-----------------------------------------------------------------

lensPortInterface *lensManager::getLensPort()
{
    return &the_lens_port;
}

//-----------------------------------------------------------------
errorCodes lensManager::initLens()
{
    the_ui->displayNotReady();
    _turnOnPower();

    //

    unsigned int n = 0;
    errorCodes err;

    while (true)
    {
        err = _sendCR(false);
        if (err == SUCCESS)
            break;

        if (n >= MAX_INIT_ATTEMPTS)
        {
            the_ui->reportError(err);
            the_ui->displayStartupError();
        }

        n++;
    }

    err = _sendCR(true);
    if (err != SUCCESS)
    {
        the_ui->reportError(err);
        the_ui->displayStartupError();
    }

    the_ui->displayReady();

    return SUCCESS;
}

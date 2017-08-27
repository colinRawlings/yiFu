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
    FLmin_mm = 0;
    FLmax_mm = 0;

    //

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
errorCodes lensManager::_sendCR(msgSpeed speed)
{
    unsigned int CRLength = 3;
    uint8_t CRmsg[] = {0x00, 0x0A, 0x00};
    uint8_t CRAnswer[CRLength];
    unsigned int answerLength = 0;

    //

    if (errorCodes err = the_lens_port.setMsg(CRmsg, CRLength))
        return err;

    switch (speed)
    {
    case (SLOW):
        if (errorCodes err = the_lens_port.sendSlowMsg())
            return err;
    case (FAST):
        if (errorCodes err = the_lens_port.sendFastMsg())
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
errorCodes lensManager::_activateLens()
{
    unsigned int msgLength = 8;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    msg[0] = 0x80;
    msg[1] = 0x0A;

    for (unsigned int i = 2; i < msgLength; i++)
        msg[i] = 0x00;

    //

    if (errorCodes err = the_lens_port.setMsg(msg, msgLength))
        return err;

    if (errorCodes err = the_lens_port.sendFastMsg())
        return err;

    //

    if (errorCodes err = the_lens_port.getAnswer(answer, answerLength))
        return err;

    FLmin_mm = answer[4];
    FLmax_mm = answer[6];

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensManager::_activateISControl()
{
    unsigned int msgLength = 8;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    //

    msg[0] = 0x91;
    msg[1] = 0xA7;
    msg[2] = 0x58;
    msg[3] = 0x00;

    msg[4] = 0x91;
    msg[5] = 0xA7;
    msg[6] = 0x58;
    msg[7] = 0x00;

    //

    if (errorCodes err = the_lens_port.setMsg(msg, msgLength))
        return err;

    if (errorCodes err = the_lens_port.sendFastMsg())
        return err;

    //

    if (errorCodes err = the_lens_port.getAnswer(answer, answerLength))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensManager::_activateTelePhotoMFControl()
{
    unsigned int msgLength = 8;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    //

    msg[0] = 0xFB;
    msg[1] = 0x1B;
    msg[2] = 0xFB;
    msg[3] = 0x1B;

    msg[4] = 0x91;
    msg[5] = 0xAA;
    msg[6] = 0x00;
    msg[7] = 0x00;

    //

    if (errorCodes err = the_lens_port.setMsg(msg, msgLength))
        return err;

    if (errorCodes err = the_lens_port.sendFastMsg())
        return err;

    //

    if (errorCodes err = the_lens_port.getAnswer(answer, answerLength))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensManager::_activatePrimeMFControl()
{
    unsigned int msgLength = 8;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    //

    msg[0] = 0xFB;
    msg[1] = 0x1B;
    msg[2] = 0xFB;
    msg[3] = 0x1B;

    msg[4] = 0x91;
    msg[5] = 0x29;
    msg[6] = 0x00;
    msg[7] = 0x00;

    //

    if (errorCodes err = the_lens_port.setMsg(msg, msgLength))
        return err;

    if (errorCodes err = the_lens_port.sendFastMsg())
        return err;

    //

    if (errorCodes err = the_lens_port.getAnswer(answer, answerLength))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes lensManager::_activateLensControls()
{
    if (FLmax_mm == 0 || FLmin_mm == 0)
        return LENS_MAN_BAD_FL;

    if (FLmin_mm == FLmax_mm)
    {
        if (errorCodes err = _activatePrimeMFControl())
            return err;
    }
    else
    {
        if (errorCodes err = _activateISControl())
            return err;

        if (errorCodes err = _activateTelePhotoMFControl())
            return err;
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
// public methods
//-----------------------------------------------------------------

void lensManager::getLensFocalLengths(int &minFocalLength_mm, int &maxFocalLength_mm)
{
    minFocalLength_mm = FLmin_mm;
    maxFocalLength_mm = FLmax_mm;
}

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
        err = _sendCR(SLOW);
        if (err == SUCCESS)
            break;

        if (n++ >= MAX_INIT_ATTEMPTS)
        {
            the_ui->reportError(err);
            the_ui->displayStartupError();
        }
    }

    err = _sendCR(FAST);
    if (err != SUCCESS)
    {
        the_ui->reportError(err);
        the_ui->displayStartupError();
    }

    the_ui->displayReady();

    //

    if (err = _activateLens())
        return err;

    the_ui->reportFocalLengths();

    if (err = _activateLensControls())
        return err;

    return SUCCESS;
}

#include "lensInitializer.h"

#include "lensPortInterface.h"

#include "HWdefs.h"
#include "SWdefs.h"

#include "errors.h"
#include "Arduino.h"

//-----------------------------------------------------------------
// Class Definition: Construct
//-----------------------------------------------------------------
lensInitializer::lensInitializer()
{
    the_lens_port = NULL;

    FLmin_mm = 0;
    FLmax_mm = 0;

    //

    _initPower();
}

//-----------------------------------------------------------------
void lensInitializer::setTheLensPort(lensPortInterface *the_lens_port_)
{
    the_lens_port = the_lens_port_;
}

//-----------------------------------------------------------------
// Class Definition: Power
//-----------------------------------------------------------------
void lensInitializer::_initPower()
{
    pinMode(lensPowerPin, OUTPUT);
}

//-----------------------------------------------------------------
void lensInitializer::_turnOnPower()
{
    digitalWrite(lensPowerPin, LPOW_ON);
}

//-----------------------------------------------------------------
void lensInitializer::_turnOffPower()
{
    digitalWrite(lensPowerPin, LPOW_OFF);
}

//-----------------------------------------------------------------
// Class Definition: Init
//-----------------------------------------------------------------

errorCode lensInitializer::_sendCR(msgSpeed speed)
{
    if (the_lens_port == NULL)
        return LENS_INIT_LENS_PORT_UNSET;

    //

    unsigned int CRLength = 3;
    uint8_t CRmsg[] = {0x00, 0x0A, 0x00};
    uint8_t CRAnswer[CRLength];
    unsigned int answerLength = 0;

    //

    if (errorCode err = the_lens_port->setMsg(CRmsg, CRLength))
        return err;

    switch (speed)
    {
    case (SLOW):
        if (errorCode err = the_lens_port->sendSlowMsg())
            return err;
    case (FAST):
        if (errorCode err = the_lens_port->sendFastMsg())
            return err;
    }

    //

    if (errorCode err = the_lens_port->getAnswer(CRAnswer, answerLength))
        return err;

    if (answerLength != CRLength)
        return LENS_INIT_BAD_CR_LENGTH;

    if (CRAnswer[2] != 0xAA)
        return LENS_INIT_BAD_CR_RESPONSE;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCode lensInitializer::_activateLens()
{
    if (the_lens_port == NULL)
        return LENS_INIT_LENS_PORT_UNSET;

    //

    unsigned int msgLength = 8;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    msg[0] = 0x80;
    msg[1] = 0x0A;

    for (unsigned int i = 2; i < msgLength; i++)
        msg[i] = 0x00;

    //

    if (errorCode err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (errorCode err = the_lens_port->sendFastMsg())
        return err;

    //

    if (errorCode err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    FLmin_mm = answer[4];
    FLmax_mm = answer[6];

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCode lensInitializer::_activateISControl()
{
    if (the_lens_port == NULL)
        return LENS_INIT_LENS_PORT_UNSET;

    //

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

    if (errorCode err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (errorCode err = the_lens_port->sendFastMsg())
        return err;

    //

    if (errorCode err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCode lensInitializer::_activateTelePhotoMFControl()
{
    if (the_lens_port == NULL)
        return LENS_INIT_LENS_PORT_UNSET;

    //

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

    if (errorCode err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (errorCode err = the_lens_port->sendFastMsg())
        return err;

    //

    if (errorCode err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCode lensInitializer::_activatePrimeMFControl()
{
    if (the_lens_port == NULL)
        return LENS_INIT_LENS_PORT_UNSET;

    //

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

    if (errorCode err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (errorCode err = the_lens_port->sendFastMsg())
        return err;

    //

    if (errorCode err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCode lensInitializer::_activateLensControls()
{
    if (FLmax_mm == 0 || FLmin_mm == 0)
        return LENS_INIT_BAD_FL;

    if (FLmin_mm == FLmax_mm)
    {
        if (errorCode err = _activatePrimeMFControl())
            return err;
    }
    else
    {
        if (errorCode err = _activateISControl())
            return err;

        if (errorCode err = _activateTelePhotoMFControl())
            return err;
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
void lensInitializer::getLensFocalLengths(int &minFocalLength_mm, int &maxFocalLength_mm)
{
    minFocalLength_mm = FLmin_mm;
    maxFocalLength_mm = FLmax_mm;
}

//-----------------------------------------------------------------
errorCode lensInitializer::initLens()
{
    unsigned int n = 0;
    errorCode err;

    _turnOnPower();

    while (true)
    {
        err = _sendCR(SLOW);
        if (err == SUCCESS)
            break;

        if (n++ >= MAX_INIT_ATTEMPTS)
            return LENS_INIT_SLOW_CR_FAILED;
    }

    if (err = _sendCR(FAST))
        return err;

    //

    if (err = _activateLens())
        return err;

    if (err = _activateLensControls())
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCode lensInitializer::resetLens()
{

    _turnOffPower();

    delay(DT_TIME_OFF_RESET_LENS_MS);

    _turnOnPower();

    //

    if (errorCode err = initLens())
        return err;

    return SUCCESS;
}
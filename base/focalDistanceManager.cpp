
#include "focalDistanceManager.h"

#include "lensPortInterface.h"

#include "utils.h"

#include "Arduino.h"

//-----------------------------------------------------------------
// Class Definition: Construct
//-----------------------------------------------------------------

focalDistanceManager::focalDistanceManager()
{
    focalDistanceMemoryPlus = 0;
    focalDistanceMemoryPlusSet = false;

    focalDistanceMemoryMinus = 0;
    focalDistanceMemoryMinusSet = false;
}

//-----------------------------------------------------------------
void focalDistanceManager::setTheLensPort(lensPortInterface *the_lens_port_)
{
    the_lens_port = the_lens_port_;
}

//-----------------------------------------------------------------
// Class Definition: Public
//-----------------------------------------------------------------

errorCodes focalDistanceManager::gotoInfFocalDistance()
{
    unsigned int msgLength = 4;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    //

    msg[0] = 0x05;
    msg[1] = 0x00;
    msg[2] = 0x00;
    msg[3] = 0x00;

    //

    if (errorCodes err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (errorCodes err = the_lens_port->sendFastMsg())
        return err;

    //

    if (errorCodes err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes focalDistanceManager::gotoMinFocalDistance()
{
    unsigned int msgLength = 4;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    //

    msg[0] = 0x06;
    msg[1] = 0x00;
    msg[2] = 0x00;
    msg[3] = 0x00;

    //

    if (errorCodes err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (errorCodes err = the_lens_port->sendFastMsg())
        return err;

    //

    if (errorCodes err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCodes focalDistanceManager::getFocalDistance(int &focalDistance)
{
    focalDistance = 0;

    //

    unsigned int msgLength = 3;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    //

    msg[0] = 0xC0;
    msg[1] = 0x00;
    msg[2] = 0x00;

    //

    if (errorCodes err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (errorCodes err = the_lens_port->sendFastMsg())
        return err;

    //

    if (errorCodes err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    focalDistance = convertBytesToInt(answer[2], answer[1]);

    return SUCCESS;
}
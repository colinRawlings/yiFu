#include "focalLengthManager.h"

#include "lensPortInterface.h"

#include "utils.h"
#include "errors.h"

//-----------------------------------------------------------------
// Class Definition: Construct
//-----------------------------------------------------------------

focalLengthManager::focalLengthManager()
{
    the_lens_port = NULL;
}

//-----------------------------------------------------------------
void focalLengthManager::setTheLensPort(lensPortInterface *the_lens_port_)
{
    the_lens_port = the_lens_port_;
}

//-----------------------------------------------------------------
// Class Definition: Operations
//-----------------------------------------------------------------

errorCodes focalLengthManager::getFocalLength(int &focalLength)
{
    focalLength = 0;

    //
    if (the_lens_port == NULL)
        return FL_MAN_LENS_PORT_UNSET;

    //

    unsigned int msgLength = 3;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    //

    msg[0] = 0xA0;
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

    focalLength = convertBytesToInt(answer[2], answer[1]);

    return SUCCESS;
}
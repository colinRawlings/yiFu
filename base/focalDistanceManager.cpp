
#include "focalDistanceManager.h"

#include "lensPortInterface.h"

#include "utils.h"

#include "SWdefs.h"
#include "errors.h"
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

    the_lens_port = NULL;
}

//-----------------------------------------------------------------
void focalDistanceManager::setTheLensPort(lensPortInterface *the_lens_port_)
{
    the_lens_port = the_lens_port_;
}

//-----------------------------------------------------------------
// Class Definition: Core
//-----------------------------------------------------------------

ErrorCode focalDistanceManager::_waitMoveComplete()
{

    unsigned long T0_us = micros(); // start time for move watchdog
    unsigned long dT_us;
    unsigned long timeout_us = DT_FOCUS_MOVE_TIMEOUT_MS * 1000;
    focusStatus theStatus;

    while (true)
    {
        dT_us = micros() - T0_us;
        if (dT_us > timeout_us)
            return FOCUS_MOVE_TIMEOUT;

        if (ErrorCode err = _getStatus(theStatus))
            return err;

        if (theStatus.focusMotorMoving == false)
            break;
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
ErrorCode focalDistanceManager::_getStatus(focusStatus &theStatus)
{
    if (the_lens_port == NULL)
        return FD_MAN_LENS_PORT_UNSET;

    //

    unsigned int msgLength = 3;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    //

    msg[0] = 0x90;
    msg[1] = 0xB9;
    msg[2] = 0x00;

    //

    if (ErrorCode err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (ErrorCode err = the_lens_port->sendFastMsg())
        return err;

    //

    if (ErrorCode err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    if (ErrorCode err = bitOfByte(answer[2], FOCUS_STATUS_AUTO_FOCUS_BIT, theStatus.autoFocusMode))
        return err;

    if (ErrorCode err = bitOfByte(answer[2], FOCUS_STATUS_FOCUS_MOVE_BIT, theStatus.focusMotorMoving))
        return err;

    if (ErrorCode err = bitOfByte(answer[2], FOCUS_STATUS_FOCUS_ACCEL_BIT, theStatus.focusMotorAccelerating))
        return err;

    if (ErrorCode err = bitOfByte(answer[2], FOCUS_STATUS_AT_END_STOP_BIT, theStatus.focusMotorAtEndStop))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
ErrorCode focalDistanceManager::gotoMinFocalDistance()
{
    if (the_lens_port == NULL)
        return FD_MAN_LENS_PORT_UNSET;

    //

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

    if (ErrorCode err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (ErrorCode err = the_lens_port->sendFastMsg())
        return err;

    //

    if (ErrorCode err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    if (ErrorCode err = _waitMoveComplete())
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
ErrorCode focalDistanceManager::gotoInfFocalDistance()
{
    if (the_lens_port == NULL)
        return FD_MAN_LENS_PORT_UNSET;

    //

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

    if (ErrorCode err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (ErrorCode err = the_lens_port->sendFastMsg())
        return err;

    //

    if (ErrorCode err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    if (ErrorCode err = _waitMoveComplete())
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
ErrorCode focalDistanceManager::getFocalDistance(int &focalDistance)
{
    focalDistance = 0;

    //

    if (the_lens_port == NULL)
        return FD_MAN_LENS_PORT_UNSET;

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

    if (ErrorCode err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (ErrorCode err = the_lens_port->sendFastMsg())
        return err;

    //

    if (ErrorCode err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    focalDistance = convertBytesToInt(answer[2], answer[1]);

    return SUCCESS;
}

//-----------------------------------------------------------------
ErrorCode focalDistanceManager::stepFocalDistance(int step)
{
    if (the_lens_port == NULL)
        return FD_MAN_LENS_PORT_UNSET;

    //

    byte mostSignificantByte = (step & 0xFF00) >> 8;
    byte leastSignificantByte = step & 0xFF;

    //

    unsigned int msgLength = 3;
    uint8_t msg[msgLength];
    uint8_t answer[msgLength];
    unsigned int answerLength = 0;

    //

    msg[0] = 0x44;
    msg[1] = mostSignificantByte;
    msg[2] = leastSignificantByte;

    //

    if (ErrorCode err = the_lens_port->setMsg(msg, msgLength))
        return err;

    if (ErrorCode err = the_lens_port->sendFastMsg())
        return err;

    //

    if (ErrorCode err = the_lens_port->getAnswer(answer, answerLength))
        return err;

    if (ErrorCode err = _waitMoveComplete())
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
ErrorCode focalDistanceManager::setFocalDistance(int newFocalDistance)
{
    int currentFocalDistance;

    if (ErrorCode err = getFocalDistance(currentFocalDistance))
        return err;

    int step = newFocalDistance - currentFocalDistance;

    if (ErrorCode err = stepFocalDistance(step))
        return err;

    return SUCCESS;
}

//-----------------------------------------------------------------
// Class Definitions: Focus Pull
//-----------------------------------------------------------------

void focalDistanceManager::setFocalDistanceMemoryPlus(int focalDistance)
{
    focalDistanceMemoryPlus = focalDistance;
    focalDistanceMemoryPlusSet = true;
}

//-----------------------------------------------------------------
void focalDistanceManager::setFocalDistanceMemoryMinus(int focalDistance)
{
    focalDistanceMemoryMinus = focalDistance;
    focalDistanceMemoryMinusSet = true;
}

//-----------------------------------------------------------------
ErrorCode focalDistanceManager::gotoFocalDistancePlus()
{
    if (focalDistanceMemoryPlusSet)
    {
        if (ErrorCode err = setFocalDistance(focalDistanceMemoryPlus))
            return err;
    }
    else
    {
        if (ErrorCode err = gotoInfFocalDistance())
            return err;
    }
}

//-----------------------------------------------------------------
ErrorCode focalDistanceManager::gotoFocalDistanceMinus()
{
    if (focalDistanceMemoryMinusSet)
    {
        if (ErrorCode err = setFocalDistance(focalDistanceMemoryMinus))
            return err;
    }
    else
    {
        if (ErrorCode err = gotoMinFocalDistance())
            return err;
    }
}
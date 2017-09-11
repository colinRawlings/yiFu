#include "lensManager.h"

#include "lensPortInterface.h"
#include "focalDistanceManagerInterface.h"
#include "lensInitializerInterface.h"

#include "errors.h"
#include "Arduino.h"

#include "HWdefs.h"
#include "SWdefs.h"

//-----------------------------------------------------------------
// Class Definition: Construct
//-----------------------------------------------------------------
lensManager::lensManager()
    : the_lens_port(),
      the_lens_initializer(),
      the_fd_manager(),
      the_av_manager()

{
    the_fd_manager.setTheLensPort(&the_lens_port);
    the_lens_initializer.setTheLensPort(&the_lens_port);
    the_av_manager.setTheLensPort(&the_lens_port);

    the_av_manager.setTheLensInitializer(&the_lens_initializer);
}

//-----------------------------------------------------------------
// public methods
//-----------------------------------------------------------------
focalDistanceManagerInterface *lensManager::getFocalDistanceManager()
{
    return &the_fd_manager;
}

//-----------------------------------------------------------------
lensInitializerInterface *lensManager::getLensInitializer()
{
    return &the_lens_initializer;
}

//-----------------------------------------------------------------
apertureManagerInterface *lensManager::getApertureManager()
{
    return &the_av_manager;
}

//-----------------------------------------------------------------
errorCode lensManager::sendLensCommand(lensCommand cmd)
{
    errorCode err;

    if (err = the_lens_port.setMsg(cmd.msg, cmd.msgLength))
        return err;

    //

    if (cmd.speed == SLOW)
        err = the_lens_port.sendSlowMsg();
    else if (cmd.speed == FAST)
        err = the_lens_port.sendFastMsg();
    else
        err = LENS_MAN_BAD_MSG_SPEED;

    if (err != SUCCESS)
        return err;

    //

    return SUCCESS;
}

//-----------------------------------------------------------------
errorCode lensManager::getLensConversation(lensConversation &conv)
{
    errorCode err;

    if (err = the_lens_port.getMsg(conv.msg, conv.msgLength))
        return err;

    //

    err = the_lens_port.getAnswer(conv.answer, conv.msgLength);
    if (err == LENS_PORT_NO_ANSWER_AVAILABLE)
        conv.answerAvailable = false;
    else
        conv.answerAvailable = true;

    if ((err != SUCCESS) && (err != LENS_PORT_NO_ANSWER_AVAILABLE))
        return err;

    //

    if (err = the_lens_port.getMsgSpeed(conv.speed))
        return err;

    return SUCCESS;
}
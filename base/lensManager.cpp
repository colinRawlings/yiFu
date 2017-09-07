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
errorCodes lensManager::getLensConversation(lensConversation &conv)
{
    errorCodes err;

    if (err = the_lens_port.getMsg(conv.msg, conv.msgLength))
        return err;

    err = the_lens_port.getAnswer(conv.answer, conv.msgLength);
    if (err == LENS_PORT_NO_ANSWER_AVAILABLE)
        conv.answerAvailable = false;
    else if (err != SUCCESS)
        return err;

    if (err = the_lens_port.getMsgSpeed(conv.speed))
        return err;
}
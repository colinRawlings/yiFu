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
      the_fd_manager(),
      the_lens_initializer()

{
    the_fd_manager.setTheLensPort(&the_lens_port);
    the_lens_initializer.setTheLensPort(&the_lens_port);
}

//-----------------------------------------------------------------
// public methods
//-----------------------------------------------------------------

lensPortInterface *lensManager::getLensPort()
{
    return &the_lens_port;
}

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

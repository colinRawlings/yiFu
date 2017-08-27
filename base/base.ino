#include "Arduino.h"

#include "UIInterface.h"
#include "lensManagerInterface.h"

#include "UI.h"
#include "lensManager.h"

#include "errors.h"

//-----------------------------------------------------------------
UIInterface *the_ui = new UI(true);
lensManagerInterface *the_lens_manager = new lensManager();

//-----------------------------------------------------------------
void setup()
{
    initSerialPort();

    the_ui->setSerialPort(&Serial);
    the_ui->setLensManager(the_lens_manager);

    the_lens_manager->setUI(the_ui);

    errorCodes err = the_lens_manager->initLens();

    the_ui->displayLensConversation();

    //
}

//-----------------------------------------------------------------
void loop()
{
    // ui.displayLensConversation();

    // ui.displayLensConversation();

    performUpdates();
}

//-----------------------------------------------------------------
void performUpdates()
{
    the_ui->update();
}

//-----------------------------------------------------------------
void initSerialPort()
{
    Serial.begin(115200);
    delay(1000);
    Serial.println("Initialising the serial port ...");
    delay(1000);
    Serial.println("/");
    delay(1000);
    Serial.println("Initialised the serial port.");
}

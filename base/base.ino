#include "Arduino.h"

#include "UIInterface.h"
#include "lensManagerInterface.h"
#include "lensInitializerInterface.h"

#include "UI.h"
#include "lensManager.h"

#include "errors.h"

//-----------------------------------------------------------------
// Setup functions
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

//-----------------------------------------------------------------

//-----------------------------------------------------------------
// Main
//-----------------------------------------------------------------
UIInterface *the_ui = new UI(true);
lensManagerInterface *the_lens_manager = new lensManager();

//-----------------------------------------------------------------
void setup()
{
    initSerialPort();

    the_ui->setSerialPort(&Serial);
    the_ui->setLensManager(the_lens_manager);

    the_ui->initLens();
}

//-----------------------------------------------------------------
void loop()
{
    the_ui->update();
}

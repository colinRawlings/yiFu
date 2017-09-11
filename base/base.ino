#include "Arduino.h"

#include "UIInterface.h"
#include "lensManagerInterface.h"
#include "lensInitializerInterface.h"

#include "UI.h"
#include "lensManager.h"

#include "errors.h"

//-----------------------------------------------------------------
// Main
//-----------------------------------------------------------------
UIInterface *the_ui = new UI();
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
    the_ui->checkInputs();
    the_ui->update();
}

//-----------------------------------------------------------------
// Setup functions
//-----------------------------------------------------------------
void initSerialPort()
{

    Serial.begin(115200);

    //

    unsigned long T0 = millis();
    while (!Serial)
    {
        delayMicroseconds(10);

        if ((millis() - T0) > DT_WAIT_FOR_SERIAL_PORT_MS)
            return; // give up
    }

    delayMicroseconds(DT_INIT_SERIAL_WAIT_VSCODE_TEST_MSG);

    String initMsg = "";

    while (Serial.available() > 0)
        initMsg += (char)Serial.read();

    Serial.println("Initialised the serial port, received: '" + initMsg + "'");
}
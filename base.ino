#include "HWdefs.h"
#include "lensInterface.h"

#include "Arduino.h"

lensInterface theLens = lensInterface(MOSI_BIT, MISO_BIT, CLK_BIT);

void setup()
{
    Serial.begin(115200);

    theLens.setSerialPort(&Serial);
}

void loop()
{
    theLens.sayHi();
}

void performUpdates()
{
}
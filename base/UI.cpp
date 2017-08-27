#include "UI.h"

#include "lensManagerInterface.h"
#include "lensPortInterface.h"

#include "HWdefs.h"
#include "SWdefs.h"

#include "errors.h"

#include "Arduino.h"

//-----------------------------------------------------------------
// Class Definition
//-----------------------------------------------------------------
UI::UI(bool echoOn_)
    : status_led(LEDStatusPin, invertedLEDs),
      error_led(LEDErrPin, invertedLEDs, ERR_TIMEOUT_US),
      set_switch(switchSetPin, invertedSwitches),
      hold_switch(switchHoldPin, invertedSwitches),
      av_switch(switchAvPin, invertedSwitches),
      plus_switch(switchPlusPin, invertedSwitches),
      minus_switch(switchMinusPin, invertedSwitches)
{
    the_lens_manager = NULL;
    the_serial_port = NULL;

    echoOn = echoOn_;
}

//-----------------------------------------------------------------
void UI::setLensManager(lensManagerInterface *the_lens_manager_)
{
    the_lens_manager = the_lens_manager_;
}

//-----------------------------------------------------------------
void UI::setSerialPort(Stream *the_serial_port_)
{

    the_serial_port = the_serial_port_;
    the_serial_port->println("UI added the serial port");
}

//-----------------------------------------------------------------
void UI::update()
{

    // get commamnds from serial port

    // get commands from switches

    error_led.update();
}

//-----------------------------------------------------------------
// Class Definition: LEDs
//-----------------------------------------------------------------
void UI::displayStartupError()
{
    the_serial_port->println("Startup Failed");

    while (true)
    {
        error_led.turnOn();
        delay(200);
        error_led.turnOff();
        delay(200);
    }
}

//-----------------------------------------------------------------
void UI::displayNotReady()
{
    status_led.turnOff();
}

//-----------------------------------------------------------------
void UI::displayEndStop()
{
    error_led.turnOn();
    status_led.turnOff();

    for (unsigned int p = 0; p < N_BLINKS_AT_END_STOP; p++)
    {
        error_led.toggle();
        status_led.toggle();

        delay(DT_BLINKS_AT_END_STOP_MS);
    }

    error_led.turnOff();
    status_led.turnOn();
}

//-----------------------------------------------------------------
void UI::displayReady()
{
    status_led.turnOn();
}

//-----------------------------------------------------------------
// Class Definition: Write to Serial Port
//-----------------------------------------------------------------

void UI::_printErrorCode(errorCodes theErrorCode)
{
    String msg = errorCodeToString(theErrorCode);
    the_serial_port->println("Error: " + msg);
}

//-----------------------------------------------------------------
void UI::reportError(errorCodes theErrorCode)
{
    error_led.activate();
    _printErrorCode(theErrorCode);

    //

    the_serial_port->println("Last message:");
    errorCodes err = displayLensConversation();
    if (err != SUCCESS)
        _printErrorCode(err);
}

//-----------------------------------------------------------------
int UI::_writeHexSequenceToSerial(uint8_t sequence[], unsigned int length)
{

    for (unsigned int i = 0; i < length; i++)
    {
        the_serial_port->print("Ox");
        if (sequence[i] < 16)
            the_serial_port->print("0");

        the_serial_port->print(sequence[i], HEX);

        if (i < length - 1)
            the_serial_port->print(",");
    }
    the_serial_port->println(".");

    return 0;
}

//-----------------------------------------------------------------
errorCodes UI::displayLensConversation()
{
    // TDOD check serial port is open
    uint8_t msgBuffer[MSG_BUFFER_LENGTH];
    unsigned int msgLength;

    lensPortInterface *the_lens_port = the_lens_manager->getLensPort();

    if (errorCodes err = the_lens_port->getMsgLength(msgLength))
        return err;

    if (msgLength == 0)
        return UI_MSG_BUFFER_EMPTY;

    if (errorCodes err = the_lens_port->getMsg(msgBuffer, msgLength))
        return err;

    //

    the_serial_port->println("----------------");
    the_serial_port->print("->the_lens_port: ");

    _writeHexSequenceToSerial(msgBuffer, msgLength);

    //

    if (errorCodes err = the_lens_port->getAnswer(msgBuffer, msgLength))
        return err;

    the_serial_port->print("<-the_lens_port: ");

    if (errorCodes err = _writeHexSequenceToSerial(msgBuffer, msgLength))
        return err;

    the_serial_port->println("----------------");

    //

    return SUCCESS;
}

#include "UI.h"

#include "lensManagerInterface.h"
#include "lensPortInterface.h"
#include "focalDistanceManagerInterface.h"

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
    if ((plus_switch.getState() == PRESSED) && (set_switch.getState() == UNPRESSED))
        _gotoFocalDistancePlus();

    if ((minus_switch.getState() == PRESSED) && (set_switch.getState() == UNPRESSED))
        _gotoFocalDistanceMinus();

    if ((plus_switch.getState() == PRESSED) && (set_switch.getState() == PRESSED))
        _setFocalDistanceMemoryPlus();

    if ((minus_switch.getState() == PRESSED) && (set_switch.getState() == PRESSED))
        _setFocalDistanceMemoryMinus();

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
void UI::_displayFocalDistanceMemorySet()
{
    status_led.turnOff();

    for (unsigned int p = 0; p < N_BLINKS_AT_FOCUS_SET; p++)
    {
        status_led.toggle();
        delay(DT_BLINKS_AT_FOCUS_SET_MS);
    }

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
void UI::_addHrule()
{
    the_serial_port->println("-------------------------------------------------------");
}

//-----------------------------------------------------------------
void UI::reportFocalLengths()
{
    int FLmin_mm;
    int FLmax_mm;

    the_lens_manager->getLensFocalLengths(FLmin_mm, FLmax_mm);

    _addHrule();

    the_serial_port->print("Focal Length range: min=");
    the_serial_port->print(FLmin_mm);
    the_serial_port->print("mm, max=");
    the_serial_port->print(FLmax_mm);
    the_serial_port->println("mm");

    _addHrule();
}

//-----------------------------------------------------------------
errorCodes UI::displayLensConversation()
{
    // TDOD check serial port is open
    uint8_t msgBuffer[MSG_BUFFER_LENGTH];
    unsigned int msgLength;
    msgSpeed speed;

    lensPortInterface *the_lens_port = the_lens_manager->getLensPort();

    if (errorCodes err = the_lens_port->getMsgLength(msgLength))
        return err;

    if (msgLength == 0)
        return UI_MSG_BUFFER_EMPTY;

    if (errorCodes err = the_lens_port->getMsg(msgBuffer, msgLength))
        return err;

    //

    _addHrule();
    the_serial_port->print("->the_lens_port: ");

    _writeHexSequenceToSerial(msgBuffer, msgLength);

    //

    if (errorCodes err = the_lens_port->getAnswer(msgBuffer, msgLength))
        return err;

    the_serial_port->print("<-the_lens_port: ");

    if (errorCodes err = _writeHexSequenceToSerial(msgBuffer, msgLength))
        return err;

    if (errorCodes err = the_lens_port->getMsgSpeed(speed))
        return err;

    switch (speed)
    {
    case SLOW:
        the_serial_port->println("(slow mode)");
    case FAST:
        the_serial_port->println("(fast mode)");
    }

    _addHrule();

    //

    return SUCCESS;
}

//-----------------------------------------------------------------
// Class Definition: operations
//-----------------------------------------------------------------

void UI::_gotoFocalDistancePlus()
{
    status_led.turnOff();

    focalDistanceManagerInterface *the_fd_manager = the_lens_manager->getFocalDistanceManager();
    if (errorCodes err = the_fd_manager->gotoFocalDistancePlus())
        reportError(err);

    //

    int fd;
    if (errorCodes err = the_fd_manager->getFocalDistance(fd))
        reportError(err);

    the_serial_port->print("Moved to + focal distance: ");
    the_serial_port->println(fd);

    while (plus_switch.getState() == PRESSED)
        ;

    status_led.turnOn();
}

//-----------------------------------------------------------------
void UI::_gotoFocalDistanceMinus()
{
    status_led.turnOff();

    focalDistanceManagerInterface *the_fd_manager = the_lens_manager->getFocalDistanceManager();
    if (errorCodes err = the_fd_manager->gotoFocalDistanceMinus())
        reportError(err);

    //

    int fd;
    if (errorCodes err = the_fd_manager->getFocalDistance(fd))
        reportError(err);

    the_serial_port->print("Moved to - focal distance: ");
    the_serial_port->println(fd);

    while (minus_switch.getState() == PRESSED)
        ;

    status_led.turnOn();
}

//-----------------------------------------------------------------
void UI::_setFocalDistanceMemoryMinus()
{

    focalDistanceManagerInterface *the_fd_manager = the_lens_manager->getFocalDistanceManager();

    int fd;
    if (errorCodes err = the_fd_manager->getFocalDistance(fd))
        reportError(err);

    the_fd_manager->setFocalDistanceMemoryMinus(fd);

    status_led.turnOff();
    while ((minus_switch.getState() == PRESSED) || (set_switch.getState() == PRESSED))
        ;

    _displayFocalDistanceMemorySet();
}

//-----------------------------------------------------------------
void UI::_setFocalDistanceMemoryPlus()
{

    focalDistanceManagerInterface *the_fd_manager = the_lens_manager->getFocalDistanceManager();

    int fd;
    if (errorCodes err = the_fd_manager->getFocalDistance(fd))
        reportError(err);

    the_fd_manager->setFocalDistanceMemoryPlus(fd);

    status_led.turnOff();
    while ((plus_switch.getState() == PRESSED) || (set_switch.getState() == PRESSED))
        ;

    _displayFocalDistanceMemorySet();
}

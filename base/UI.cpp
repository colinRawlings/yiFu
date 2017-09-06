#include "UI.h"

#include "lensPortInterface.h"
#include "lensManagerInterface.h"

#include "focalDistanceManagerInterface.h"
#include "lensInitializerInterface.h"
#include "apertureManagerInterface.h"

#include "HWdefs.h"
#include "SWdefs.h"

#include "errors.h"

#include "Arduino.h"

//-----------------------------------------------------------------
// Class Definition: init
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
// Class Definition: outputs
//-----------------------------------------------------------------

void UI::_displayStartupError()
{
    the_serial_port->println("Startup Failed");

    while (true)
    {
        error_led.toggle();
        delay(DT_BLINKS_AT_STARTUP_ERROR);
    }
}

//-----------------------------------------------------------------
void UI::_displayNotReady()
{
    status_led.turnOff();
}

//-----------------------------------------------------------------
void UI::_displayEndStop()
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
void UI::_displayReady()
{
    status_led.turnOn();
}

//-----------------------------------------------------------------

void UI::_printErrorCode(errorCodes theErrorCode)
{
    String msg = errorCodeToString(theErrorCode);
    the_serial_port->println("Error: " + msg);
}

//-----------------------------------------------------------------
void UI::_reportError(errorCodes theErrorCode)
{
    error_led.activate();
    _printErrorCode(theErrorCode);
}

//-----------------------------------------------------------------
void UI::_writeHexSequenceToSerial(uint8_t sequence[], unsigned int length)
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
}

//-----------------------------------------------------------------
void UI::_addHrule()
{
    the_serial_port->println("-------------------------------------------------------");
}

//-----------------------------------------------------------------
void UI::_reportFocalLengths()
{
    if (_checkUIReadyForOperation())
        return;

    //

    int FLmin_mm;
    int FLmax_mm;

    lensInitializerInterface *the_lens_intializer = the_lens_manager->getLensInitializer();

    the_lens_intializer->getLensFocalLengths(FLmin_mm, FLmax_mm);

    _addHrule();

    the_serial_port->println("Focal Length range: min=" + String(FLmin_mm) + "mm, max=" + String(FLmax_mm) + "mm");

    _addHrule();
}

//-----------------------------------------------------------------
errorCodes UI::_displayLensConversation()
{
    if (_checkUIReadyForOperation())
        return;

    //

    lensConversation conv;

    if (errorCodes err = the_lens_manager->getLensConversation(conv))
    {
        _reportError(err);
        return;
    }

    _addHrule();
    the_serial_port->print("->the_lens_port: ");

    _writeHexSequenceToSerial(conv.msg, conv.msgLength);

    //

    if (!conv.answerAvailable)
    {
        the_serial_port->println("No answer available");
        return;
    }

    the_serial_port->print("<-the_lens_port: ");

    _writeHexSequenceToSerial(conv.answer, conv.msgLength);

    switch (conv.speed)
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
// Class Definition: inputs
//-----------------------------------------------------------------

bool UI::_allModifierSwitchesUnpressed()
{
    bool flag = true;

    flag = flag && (set_switch.getState() == UNPRESSED);
    flag = flag && (hold_switch.getState() == UNPRESSED);
    flag = flag && (av_switch.getState() == UNPRESSED);

    return flag;
}

//-----------------------------------------------------------------
// Class Definition: operations
//-----------------------------------------------------------------

int UI::_checkUIReadyForOperation()
{
    if (the_lens_manager == NULL)
    {
        _reportError(UI_LENS_MAN_UNSET);
        return 1;
    }

    if (the_serial_port == NULL)
    {
        _reportError(UI_SERIAL_PORT_UNSET);
        return 1;
    }

    return 0;
}

//-----------------------------------------------------------------
void UI::_gotoFocalDistancePlus()
{
    if (_checkUIReadyForOperation())
        return;

    //

    focalDistanceManagerInterface *the_fd_manager = the_lens_manager->getFocalDistanceManager();
    if (errorCodes err = the_fd_manager->gotoFocalDistancePlus())
    {
        _reportError(err);
        return;
    }

    //

    int fd;
    if (errorCodes err = the_fd_manager->getFocalDistance(fd))
    {
        _reportError(err);
        return;
    }

    the_serial_port->println("Moved to + focal distance: " + String(fd));
}

//-----------------------------------------------------------------
void UI::_gotoFocalDistanceMinus()
{
    if (_checkUIReadyForOperation())
        return;

    //

    focalDistanceManagerInterface *the_fd_manager = the_lens_manager->getFocalDistanceManager();
    if (errorCodes err = the_fd_manager->gotoFocalDistanceMinus())
    {
        _reportError(err);
        return;
    }

    //

    int fd;
    if (errorCodes err = the_fd_manager->getFocalDistance(fd))
    {
        _reportError(err);
        return;
    }

    the_serial_port->println("Moved to - focal distance: " + String(fd));
}

//-----------------------------------------------------------------
void UI::_setFocalDistanceMemoryMinus()
{
    if (_checkUIReadyForOperation())
        return;

    //

    focalDistanceManagerInterface *the_fd_manager = the_lens_manager->getFocalDistanceManager();

    int fd;
    if (errorCodes err = the_fd_manager->getFocalDistance(fd))
    {
        _reportError(err);
        return;
    }

    the_fd_manager->setFocalDistanceMemoryMinus(fd);
}

//-----------------------------------------------------------------
void UI::_setFocalDistanceMemoryPlus()
{
    if (_checkUIReadyForOperation())
        return;

    //

    focalDistanceManagerInterface *the_fd_manager = the_lens_manager->getFocalDistanceManager();

    int fd;
    if (errorCodes err = the_fd_manager->getFocalDistance(fd))
    {
        _reportError(err);
        return;
    }

    the_fd_manager->setFocalDistanceMemoryPlus(fd);
}

//-----------------------------------------------------------------
void UI::_reportApertureValue()
{
    if (_checkUIReadyForOperation())
        return;

    apertureManagerInterface *the_av_manager = the_lens_manager->getApertureManager();

    unsigned int av;

    errorCodes err = the_av_manager->getApertureValue_tics(av);

    String msg;
    if (err == SUCCESS)
        msg = "Aperture value: " + String(av) + "tics";
    else if (err = AV_AV_UNKNOWN)
        msg = "Aperture value: unknown";
    else
    {
        _reportError(err);
        return;
    }

    the_serial_port->println(msg);
}

//-----------------------------------------------------------------
void UI::_apertureOpenOneStep()
{
    if (_checkUIReadyForOperation())
        return;

    apertureManagerInterface *the_av_manager = the_lens_manager->getApertureManager();

    if (errorCodes err = the_av_manager->openOneStep())
    {
        _reportError(err);
        return;
    }

    _reportApertureValue();
}

//-----------------------------------------------------------------
void UI::_apertureCloseOneStep()
{
    if (_checkUIReadyForOperation())
        return;

    apertureManagerInterface *the_av_manager = the_lens_manager->getApertureManager();

    if (errorCodes err = the_av_manager->closeOneStep())
    {
        _reportError(err);
        return;
    }

    _reportApertureValue();
}

//-----------------------------------------------------------------
// Class methods: public
//-----------------------------------------------------------------
void UI::initLens()
{
    if (_checkUIReadyForOperation())
        return;

    //

    _displayNotReady();

    if (hold_switch.getState() == PRESSED)
    {
        the_serial_port->println("Booted with hold switch, skipping init");
        _displayReady();
        return;
    }

    //

    lensInitializerInterface *the_lens_initializer = the_lens_manager->getLensInitializer();

    errorCodes err = the_lens_initializer->initLens();

    if (err != SUCCESS)
    {
        _reportError(err);
        _displayStartupError();
    }

    _displayReady();

    _reportFocalLengths();
}

//-----------------------------------------------------------------
void UI::update()
{
    if ((plus_switch.getState() == PRESSED) && _allModifierSwitchesUnpressed())
    {
        status_led.turnOff();

        _gotoFocalDistancePlus();

        while (plus_switch.getState() == PRESSED)
            delayMicroseconds(10);

        status_led.turnOn();
    }

    if ((minus_switch.getState() == PRESSED) && _allModifierSwitchesUnpressed())
    {
        status_led.turnOff();

        _gotoFocalDistanceMinus();

        while (minus_switch.getState() == PRESSED)
            delayMicroseconds(10);

        status_led.turnOn();
    }

    if ((plus_switch.getState() == PRESSED) && (set_switch.getState() == PRESSED))
    {
        status_led.turnOff();

        _setFocalDistanceMemoryPlus();

        while ((plus_switch.getState() == PRESSED) || (set_switch.getState() == PRESSED))
            delayMicroseconds(10);

        _displayFocalDistanceMemorySet();
    }

    if ((minus_switch.getState() == PRESSED) && (set_switch.getState() == PRESSED))
    {
        status_led.turnOff();

        _setFocalDistanceMemoryMinus();

        while ((minus_switch.getState() == PRESSED) || (set_switch.getState() == PRESSED))
            delayMicroseconds(10);

        _displayFocalDistanceMemorySet();
    }

    if ((plus_switch.getState() == PRESSED) && (av_switch.getState() == PRESSED))
    {
        status_led.turnOff();

        _apertureOpenOneStep();

        while ((plus_switch.getState() == PRESSED) || (av_switch.getState() == PRESSED))
            delayMicroseconds(10);

        status_led.turnOn();
    }

    if ((minus_switch.getState() == PRESSED) && (av_switch.getState() == PRESSED))
    {
        status_led.turnOff();
        _apertureCloseOneStep();

        while ((minus_switch.getState() == PRESSED) || (av_switch.getState() == PRESSED))
            delayMicroseconds(10);

        status_led.turnOn();
    }

    // get commamnds from serial port

    error_led.update();
}
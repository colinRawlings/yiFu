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
#include <ctype.h>

//-----------------------------------------------------------------
// Class Definition: init
//-----------------------------------------------------------------
UI::UI()
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

void UI::_printErrorCode(errorCode theErrorCode)
{
    String msg = errorCodeToString(theErrorCode);
    the_serial_port->println("Error: " + msg);
}

//-----------------------------------------------------------------
void UI::_reportError(errorCode theErrorCode)
{
    error_led.activate();
    _printErrorCode(theErrorCode);
    _addEndComm();
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
void UI::_addEndComm()
{
    the_serial_port->println("----------------END");
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
int UI::_displayLensConversation()
{
    if (_checkUIReadyForOperation())
        return 1;

    //

    lensConversation conv;

    if (errorCode err = the_lens_manager->getLensConversation(conv))
    {
        _reportError(err);
        return 1;
    }

    _addHrule();
    the_serial_port->print("MOSI: ");

    _writeHexSequenceToSerial(conv.msg, conv.msgLength);

    //

    if (!conv.answerAvailable)
    {
        the_serial_port->println("No answer available");
        return 1;
    }

    the_serial_port->print("MISO: ");

    _writeHexSequenceToSerial(conv.answer, conv.msgLength);

    switch (conv.speed)
    {
    case SLOW:
    {
        the_serial_port->println("(slow mode)");
        break;
    }
    case FAST:
    {
        the_serial_port->println("(fast mode)");
        break;
    }
    }

    _addEndComm();

    //

    return 0;
}

//-----------------------------------------------------------------
// Class Definition: inputs
//-----------------------------------------------------------------

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
    if (errorCode err = the_fd_manager->gotoFocalDistancePlus())
    {
        _reportError(err);
        return;
    }

    //

    int fd;
    if (errorCode err = the_fd_manager->getFocalDistance(fd))
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
    if (errorCode err = the_fd_manager->gotoFocalDistanceMinus())
    {
        _reportError(err);
        return;
    }

    //

    int fd;
    if (errorCode err = the_fd_manager->getFocalDistance(fd))
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
    if (errorCode err = the_fd_manager->getFocalDistance(fd))
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
    if (errorCode err = the_fd_manager->getFocalDistance(fd))
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

    errorCode err = the_av_manager->getApertureValue_tics(av);

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

    if (errorCode err = the_av_manager->openOneStep())
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

    if (errorCode err = the_av_manager->closeOneStep())
    {
        _reportError(err);
        return;
    }

    _reportApertureValue();
}
//-----------------------------------------------------------------
void UI::_holdFocalDistance()
{
    if (_checkUIReadyForOperation())
        return;

    focalDistanceManagerInterface *the_fd_manager = the_lens_manager->getFocalDistanceManager();

    int holdFD;
    if (errorCode err = the_fd_manager->getFocalDistance(holdFD))
    {
        _reportError(err);
        return;
    }

    while (hold_switch.getState() == PRESSED)
    {
        if (errorCode err = the_fd_manager->setFocalDistance(holdFD))
        {
            _reportError(err);
            return;
        }
    }
}

//-----------------------------------------------------------------
// serial port command
//-----------------------------------------------------------------
void UI::_sendSerialPortCommandToLens()
{
    if (_checkUIReadyForOperation())
        return;

    errorCode err;

    //

    lensCommand cmd;

    if (err = _parseSerialPortInput(cmd))
    {
        _reportError(err);
        return;
    }

    //

    _addHrule();

    the_serial_port->print("Serial port request to send ");
    if (cmd.speed == FAST)
        the_serial_port->println("fast message:");
    else if (cmd.speed == SLOW)
        the_serial_port->println("slow message:");

    _writeHexSequenceToSerial(cmd.msg, cmd.msgLength);

    _addHrule();

    //

    if (err = the_lens_manager->sendLensCommand(cmd))
    {
        _reportError(err);
        return;
    }

    if (err = _displayLensConversation())
        return;
}

//-----------------------------------------------------------------
errorCode UI::_parseSerialPortInput(lensCommand &cmd)
{

    errorCode err;

    cmd.msg[0] = 0x00;
    cmd.msgLength = 0;
    cmd.speed = SLOW;

    //

    char speedChar = the_serial_port->read();
    if (speedChar == '+')
        cmd.speed = FAST;
    else if (speedChar == '-')
        cmd.speed = SLOW;
    else
    {
        _emptySerialInputBuffer();
        return UI_PARSE_SERIAL_NO_SPEED_CODE;
    }

    //

    char inputChar;
    int lowNibble;
    int highNibble;

    while (the_serial_port->available() > 0)
    {
        if (the_serial_port->available() < 2)
        {
            _emptySerialInputBuffer();
            return UI_PARSE_SERIAL_INCOMPLETE_MSG;
        }

        //

        inputChar = the_serial_port->read();
        if (err = _charToNibble(inputChar, highNibble))
        {
            _emptySerialInputBuffer();
            return err;
        }

        inputChar = the_serial_port->read();
        if (err = _charToNibble(inputChar, lowNibble))
        {
            _emptySerialInputBuffer();
            return err;
        }

        cmd.msgLength++;
        if (cmd.msgLength > LENS_BUFFER_LENGTH)
        {
            _emptySerialInputBuffer();
            return UI_PARSE_SERIAL_MSG_TOO_LONG;
        }
        cmd.msg[cmd.msgLength - 1] = 16 * highNibble + lowNibble;

        //

        if (the_serial_port->available() == 0)
            break;

        //

        inputChar = the_serial_port->read();

        if (inputChar == '\r')
        {
            _emptySerialInputBuffer();
            break;
        }

        if (inputChar != ',')
        {
            _emptySerialInputBuffer();
            return UI_PARSE_SERIAL_MISSING_SEPARATOR;
        }
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
void UI::_emptySerialInputBuffer()
{
    the_serial_port->print("discarding serial input: ");

    while (the_serial_port->available() > 0)
        the_serial_port->print((char)the_serial_port->read());

    the_serial_port->println(".");
}

//-----------------------------------------------------------------
errorCode UI::_charToNibble(char c, int &val)
{
    val = 0;

    if (isalpha(c))
    {
        c = toupper(c);
        val = c - 'A' + 10;
    }
    else
    {
        val = c - '0';
        if (val < 0 || val > 9)
            return UI_PARSE_SERIAL_NOT_HEX_CODE;
    }

    return SUCCESS;
}

//-----------------------------------------------------------------
// check inputs
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
void UI::_checkSwitches()
{
    if (hold_switch.getState() == PRESSED)
    {
        status_led.turnOff();

        _holdFocalDistance();

        while (hold_switch.getState() == PRESSED)
            update();

        status_led.turnOn();
    }

    if ((plus_switch.getState() == PRESSED) && _allModifierSwitchesUnpressed())
    {
        status_led.turnOff();

        _gotoFocalDistancePlus();

        while (plus_switch.getState() == PRESSED)
            update();

        status_led.turnOn();
    }

    if ((minus_switch.getState() == PRESSED) && _allModifierSwitchesUnpressed())
    {
        status_led.turnOff();

        _gotoFocalDistanceMinus();

        while (minus_switch.getState() == PRESSED)
            update();

        status_led.turnOn();
    }

    if ((plus_switch.getState() == PRESSED) && (set_switch.getState() == PRESSED))
    {
        status_led.turnOff();

        _setFocalDistanceMemoryPlus();

        while ((plus_switch.getState() == PRESSED) || (set_switch.getState() == PRESSED))
            update();

        _displayFocalDistanceMemorySet();
    }

    if ((minus_switch.getState() == PRESSED) && (set_switch.getState() == PRESSED))
    {
        status_led.turnOff();

        _setFocalDistanceMemoryMinus();

        while ((minus_switch.getState() == PRESSED) || (set_switch.getState() == PRESSED))
            update();

        _displayFocalDistanceMemorySet();
    }

    if ((plus_switch.getState() == PRESSED) && (av_switch.getState() == PRESSED))
    {
        status_led.turnOff();

        _apertureOpenOneStep();

        while ((plus_switch.getState() == PRESSED) || (av_switch.getState() == PRESSED))
            update();

        status_led.turnOn();
    }

    if ((minus_switch.getState() == PRESSED) && (av_switch.getState() == PRESSED))
    {
        status_led.turnOff();
        _apertureCloseOneStep();

        while ((minus_switch.getState() == PRESSED) || (av_switch.getState() == PRESSED))
            update();

        status_led.turnOn();
    }
}

//-----------------------------------------------------------------
void UI::_checkSerialPort()
{
    if (the_serial_port->available() > 0)
    {
        _sendSerialPortCommandToLens();
    }
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

    errorCode err = the_lens_initializer->initLens();

    if (err != SUCCESS)
    {
        _reportError(err);
        _displayStartupError();
    }

    _displayReady();

    _reportFocalLengths();
}

//-----------------------------------------------------------------
void UI::checkInputs()
{
    _checkSerialPort();
    _checkSwitches();
}

//-----------------------------------------------------------------
void UI::update()
{
    error_led.update();
}
#ifndef _UI_H
#define _UI_H

//
#include "UIInterface.h"

#include "pushSwitch.h"
#include "LED.h"
#include "persistentLED.h"

#include "errors.h"
#include "Arduino.h"

//
class lensManagerInterface;

// class
class UI : public UIInterface
{
private:
  Stream *the_serial_port;
  lensManagerInterface *the_lens_manager;

  persistentLED error_led;
  LED status_led;

  pushSwitch set_switch;
  pushSwitch hold_switch;
  pushSwitch av_switch;
  pushSwitch plus_switch;
  pushSwitch minus_switch;

  bool echoOn;

private:
  int _writeHexSequenceToSerial(uint8_t sequence[], unsigned int msgLength);
  void _printErrorCode(errorCodes theErrorCode);
  void _addHrule();

  void _gotoFocalDistancePlus();
  void _gotoFocalDistanceMinus();

  void _setFocalDistanceMemoryPlus();
  void _setFocalDistanceMemoryMinus();

  void _displayFocalDistanceMemorySet();

  errorCodes _displayLensConversation();
  void _reportError(errorCodes theErrorCode);
  void _displayStartupError();
  void _displayEndStop();

  void _displayNotReady();
  void _displayReady();

  void _reportFocalLengths();

public:
  UI(bool echoOn_);

  void setLensManager(lensManagerInterface *the_lens_manager_);
  void setSerialPort(Stream *the_serial_port_);

  void initLens();

  void update();
};

#endif // _UI_H
#ifndef _UI_H
#define _UI_H

//
#include "UIInterface.h"
#include "Arduino.h"

#include "pushSwitch.h"
#include "LED.h"
#include "persistentLED.h"

#include "errors.h"

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

public:
  UI(bool echoOn_);

  void setLensManager(lensManagerInterface *theLens_);
  void setSerialPort(Stream *the_serial_port_);

  void reportError(errorCodes theErrorCode);
  void displayStartupError();
  void displayEndStop();

  void displayNotReady();
  void displayReady();

  errorCodes displayLensConversation();

  void update();
};

#endif // _UI_H
#ifndef _UI_H
#define _UI_H

//
#include "UIInterface.h"
#include "lensManagerInterface.h"

#include "pushSwitch.h"
#include "LED.h"

#include "errors.h"
#include "Arduino.h"

// class
class UI : public UIInterface
{
private:
  Stream *the_serial_port;
  lensManagerInterface *the_lens_manager;
  LED error_led;
  LED status_led;

  pushSwitch set_switch;
  pushSwitch hold_switch;
  pushSwitch av_switch;
  pushSwitch plus_switch;
  pushSwitch minus_switch;

  bool lensInitialised;

private:
  // outputs
  void _writeHexSequenceToSerial(uint8_t sequence[], unsigned int msgLength);
  void _printErrorCode(ErrorCode theErrorCode);
  void _addHrule();
  void _addEndComm();

  int _displayLensConversation();
  void _displayStartupError();
  void _displayEndStop();

  void _displayNotReady();
  void _displayReady();
  void _displayFocalDistanceMemorySet();

  void _reportError(ErrorCode theErrorCode);
  void _reportFocalLengths();
  void _reportApertureValue();

  // check inputs
  bool _allModifierSwitchesUnpressed();
  void _checkSerialPort();
  void _checkSwitches();

  // serial port commands
  ErrorCode _parseSerialPortInput(lensCommand &cmd);
  ErrorCode _charToNibble(char c, int &val);
  void _emptySerialInputBuffer();

  // operations
  int _checkUIReadyForOperation();

  void _gotoFocalDistancePlus();
  void _gotoFocalDistanceMinus();

  void _setFocalDistanceMemoryPlus();
  void _setFocalDistanceMemoryMinus();

  void _holdFocalDistance();

  void _apertureOpenOneStep();
  void _apertureCloseOneStep();

  void _sendSerialPortCommandToLens();

  //

public:
  UI();

  void setLensManager(lensManagerInterface *the_lens_manager_);
  void setSerialPort(Stream *the_serial_port_);

  void initLens();

  void checkInputs();
  void update();
};

#endif // _UI_H
// SPI interface to the lens

#ifndef _UH_H
#define _UH_H

#include "persistentLED.h"
#include "LED.h"
#include "pushSwitch.h"
#include "lensManager.h"

#include "Arduino.h"

class UI
{
  private:
    Stream *SerialPort;

    persistentLED errorLED;
    LED statusLED;

    pushSwitch set;
    pushSwitch hold;
    pushSwitch Av;
    pushSwitch plus;
    pushSwitch minus;

    lensManager *lens;

  private:
    bool _cmdAvailable();
    void _readCmd(char cmd[], unsigned int cmdLength);

  public:
    UI();
    ~UI();

    void displayError();
    void displayEndStop();

    int writeMessageToSerial(char msg[], unsigned int msgLength);

    void update();
};

#endif // _UH_H
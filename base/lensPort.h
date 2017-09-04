// SPI interface to the lens

#ifndef _LENS_PORT_H
#define _LENS_PORT_H

#include "lensPortInterface.h"

#include "SWdefs.h"

#include "errors.h"
#include "Arduino.h"

//-----------------------------------------------------------------
// classes
//-----------------------------------------------------------------

class lensPort : public lensPortInterface
{
private:
  uint8_t CLK_byte;
  uint8_t MOSI_byte;

  uint8_t SPI_MOSI_buffer[LENS_BUFFER_LENGTH];
  uint8_t SPI_MISO_buffer[LENS_BUFFER_LENGTH];

  unsigned int msgLength;
  bool msgAvailable;
  msgSpeed speed;

  // debug

private:
  errorCodes _sendSlowByte(uint8_t val_MOSI, uint8_t &val_MISO);
  errorCodes _sendFastByte(uint8_t val_MOSI, uint8_t &val_MISO);

  errorCodes _waitForLens(unsigned long timeout_us);

  //

  void _disp_hexSeq(int len, uint8_t chars[LENS_BUFFER_LENGTH]);

protected:
public:
  lensPort();

  errorCodes setMsg(uint8_t msg[], unsigned int msgLength_);

  errorCodes sendFastMsg();
  errorCodes sendSlowMsg();

  errorCodes getMsgSpeed(msgSpeed &speed);
  errorCodes getMsgLength(unsigned int &msgLength_);
  errorCodes getMsg(uint8_t msg[], unsigned int &msgLength_);
  errorCodes getAnswer(uint8_t answer[], unsigned int &answerLength_);

  // debug
};

#endif // _LENS_PORT_H
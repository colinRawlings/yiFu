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
  errorCode _sendSlowByte(uint8_t val_MOSI, uint8_t &val_MISO);
  errorCode _sendFastByte(uint8_t val_MOSI, uint8_t &val_MISO);

  errorCode _waitForLens(unsigned long timeout_us);

  //

  void _disp_hexSeq(int len, uint8_t chars[LENS_BUFFER_LENGTH]);

protected:
public:
  lensPort();

  errorCode setMsg(uint8_t msg[], unsigned int msgLength_);

  errorCode sendFastMsg();
  errorCode sendSlowMsg();

  errorCode getMsgSpeed(msgSpeed &speed);
  errorCode getMsgLength(unsigned int &msgLength_);
  errorCode getMsg(uint8_t msg[], unsigned int &msgLength_);
  errorCode getAnswer(uint8_t answer[], unsigned int &answerLength_);

  // debug
};

#endif // _LENS_PORT_H
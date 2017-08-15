// SPI interface to the lens

#ifndef _LENS_INTERFACE_H
#define _LENS_INTERFACE_H

#include "Arduino.h"

//-----------------------------------------------------------------
// constants
//-----------------------------------------------------------------

#define SPI_BUFFER_LENGTH 20

//-----------------------------------------------------------------
// classes
//-----------------------------------------------------------------

class lensInterface
{
private:
  uint8_t MOSI_bit;
  uint8_t MISO_bit;
  uint8_t CLK_bit;

  uint8_t CLK_byte;
  uint8_t MOSI_byte;

  uint8_t SPI_MOSI_buffer[SPI_BUFFER_LENGTH];
  uint8_t SPI_MISO_buffer[SPI_BUFFER_LENGTH];

  unsigned int msgLength;
  bool msgAvailable;

  // debug

  Stream *theSerialPort; // eventually handled externally

private:
  int _sendSlowByte(uint8_t val_MOSI, uint8_t *val_MISO);

  int _waitForLens(unsigned long timeout_us);

  //

  void _disp_hexSeq(int len, uint8_t chars[SPI_BUFFER_LENGTH]);

protected:
public:
  int sendFastMsg();
  int sendSlowMsg();

  lensInterface(uint8_t MOSI_bit_, uint8_t MISO_bit_, uint8_t CLK_bit_);

  int setMsg(uint8_t msg[], unsigned int msgLength_);
  int getAnswer(uint8_t answer[], unsigned int &answerLength);

  // debug

  void sayHi();
  setSerialPort(Stream *theSerialPort_);

  void dispCmdResults(int commandLen, byte instrArr[], byte responseArr[], bool fastMode);
};

#endif // _LENS_INTERFACE_H
#ifndef _HW_DEFS_H
#define _HW_DEFS_H

//-----------------------------------------------------------------
// Lens Interface
//-----------------------------------------------------------------

#define SPI_OUTPORT PORTB
#define SPI_INPORT PINB
#define SPI_DIR DDRB
#define LPOW_PORT PORTB

#define MOSI_BIT 2 //Dout, MOSI
#define MISO_BIT 3 // Din, MISO
#define CLK_BIT 1  //Dclk

// define timing delays for fast and slow comm sequences
#define byte_wait_fast_us 10
#define byte_wait_slow_us 120

//-----------------------------------------------------------------
// Lens Manager
//-----------------------------------------------------------------

#define lensPowerPin 10 // i.e. D10
#define LPOW_ON LOW
#define LPOW_OFF HIGH

//-----------------------------------------------------------------
// UI
//-----------------------------------------------------------------

#define invertedLEDs false
#define LEDStatusPin 11
#define LEDErrPin 12

#define invertedSwitches true
#define switchSetPin 6
#define switchHoldPin 8
#define switchAvPin 9
#define switchPlusPin 4
#define switchMinusPin 5

#endif // _HW_DEFS_H
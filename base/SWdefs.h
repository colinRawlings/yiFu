#ifndef _SW_DEFS_H
#define _SW_DEFS_H

//-----------------------------------------------------------------
// UI
//-----------------------------------------------------------------

#define ERR_TIMEOUT_US 2000000
#define MSG_BUFFER_LENGTH 20
#define N_BLINKS_AT_END_STOP 3
#define DT_BLINKS_AT_END_STOP_MS 200

//-----------------------------------------------------------------
// lensManager
//-----------------------------------------------------------------

#define MAX_INIT_ATTEMPTS 3

//-----------------------------------------------------------------
// lensPort
//-----------------------------------------------------------------

#define SPI_BUFFER_LENGTH 20

//-----------------------------------------------------------------
// constants
//-----------------------------------------------------------------

#define DT_BETWEEN_SLOW_BYTES_US 120
#define DT_BETWEEN_FAST_BYTES_US 10
#define DT_LENS_TIMEOUT_US 30000

#endif // _SW_DEFS_H

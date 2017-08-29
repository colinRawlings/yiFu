#ifndef _SW_DEFS_H
#define _SW_DEFS_H

//-----------------------------------------------------------------
// UI
//-----------------------------------------------------------------

#define ERR_TIMEOUT_US 2000000
#define MSG_BUFFER_LENGTH 20
#define N_BLINKS_AT_END_STOP 3
#define DT_BLINKS_AT_END_STOP_MS 200

#define N_BLINKS_AT_FOCUS_SET 3
#define DT_BLINKS_AT_FOCUS_SET_MS 200

//-----------------------------------------------------------------
// lensManager
//-----------------------------------------------------------------

#define MAX_INIT_ATTEMPTS 3

//-----------------------------------------------------------------
// lensPort
//-----------------------------------------------------------------

#define SPI_BUFFER_LENGTH 20
#define DT_BETWEEN_SLOW_BYTES_US 120
#define DT_BETWEEN_FAST_BYTES_US 10
#define DT_LENS_TIMEOUT_US 30000

//-----------------------------------------------------------------
// focalDistanceManager
//-----------------------------------------------------------------

#define DT_FOCUS_MOVE_TIMEOUT_MS 100

#define FOCUS_STATUS_AUTO_FOCUS_BIT 7
#define FOCUS_STATUS_FOCUS_MOVE_BIT 2
#define FOCUS_STATUS_FOCUS_ACCEL_BIT 5
#define FOCUS_STATUS_AT_END_STOP_BIT 4

#endif // _SW_DEFS_H

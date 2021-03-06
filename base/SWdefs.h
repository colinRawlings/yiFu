#ifndef _SW_DEFS_H
#define _SW_DEFS_H

//-----------------------------------------------------------------
// Serial port
//-----------------------------------------------------------------

#define DT_WAIT_FOR_SERIAL_PORT_MS 2000         // wait this long for the serial port to start
#define DT_INIT_SERIAL_WAIT_VSCODE_TEST_MSG 100 // normally arrives after 8 us

//-----------------------------------------------------------------
// pushSwitch
//-----------------------------------------------------------------

#define DT_SWITCH_DEBOUNCE_WAIT_MS 50

//-----------------------------------------------------------------
// lensPort
//-----------------------------------------------------------------

#define LENS_BUFFER_LENGTH 20
#define DT_BETWEEN_SLOW_BYTES_US 120
#define DT_BETWEEN_FAST_BYTES_US 10
#define DT_LENS_TIMEOUT_US 30000

//-----------------------------------------------------------------
// lensInitializer
//-----------------------------------------------------------------

#define DT_TIME_OFF_RESET_LENS_MS 100

//-----------------------------------------------------------------
// lensManager
//-----------------------------------------------------------------

#define MAX_INIT_ATTEMPTS 3

//-----------------------------------------------------------------
// UI
//-----------------------------------------------------------------

#define DT_DISPLAY_ERR_MS 2000
#define MSG_BUFFER_LENGTH 20
#define N_BLINKS_AT_END_STOP 3
#define DT_BLINKS_AT_END_STOP_MS 200

#define N_BLINKS_AT_FOCUS_SET 3
#define DT_BLINKS_AT_FOCUS_SET_MS 200

#define DT_BLINKS_AT_STARTUP_ERROR_MS 200

//-----------------------------------------------------------------
// focalDistanceManager
//-----------------------------------------------------------------

#define DT_FOCUS_MOVE_TIMEOUT_MS 100

#define FOCUS_STATUS_AUTO_FOCUS_BIT 7
#define FOCUS_STATUS_FOCUS_MOVE_BIT 2
#define FOCUS_STATUS_FOCUS_ACCEL_BIT 5
#define FOCUS_STATUS_AT_END_STOP_BIT 4

//-----------------------------------------------------------------
// apertureManager
//-----------------------------------------------------------------

#define MAX_APERTURE_TICS 21
#define APERTURE_STEP_SIZE_TICS 2

#endif // _SW_DEFS_H

#ifndef _ERRORS_H
#define _ERRORS_H

#include "Arduino.h"

enum ErrorCode
{
    SUCCESS = 0,

    LENS_PORT_CLK_PULL_DOWN_TIMEOUT,
    LENS_PORT_CLK_RELEASE_TIMEOUT,
    LENS_PORT_SPI_BUFFER_TOO_SMALL,
    LENS_PORT_NO_ANSWER_AVAILABLE,

    LENS_MAN_BAD_MSG_SPEED,

    LENS_INIT_LENS_PORT_UNSET,
    LENS_INIT_BAD_CR_LENGTH,
    LENS_INIT_BAD_CR_RESPONSE,
    LENS_INIT_BAD_FL,
    LENS_INIT_SLOW_CR_FAILED,

    FD_MAN_LENS_PORT_UNSET,

    FL_MAN_LENS_PORT_UNSET,

    UI_MSG_BUFFER_EMPTY,
    UI_LENS_MAN_UNSET,
    UI_SERIAL_PORT_UNSET,
    UI_PARSE_SERIAL_NO_SPEED_CODE,
    UI_PARSE_SERIAL_INCOMPLETE_MSG,
    UI_PARSE_SERIAL_NOT_HEX_CODE,
    UI_PARSE_SERIAL_MSG_TOO_LONG,
    UI_PARSE_SERIAL_MISSING_SEPARATOR,
    UI_NOT_READY,

    UTILS_TOO_BIT_BIT_OF_BYTE,

    AV_AV_UNKNOWN,
    AV_LENS_PORT_UNSET,
    AV_LENS_INITIALIZER_UNSET,

    FOCUS_MOVE_TIMEOUT
};

String errorCodeToString(ErrorCode theErrorCode);

#endif //_ERRORS_H
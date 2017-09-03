#ifndef _ERRORS_H
#define _ERRORS_H

#include "Arduino.h"

enum errorCodes
{
    SUCCESS = 0,

    LENS_PORT_CLK_PULL_DOWN_TIMEOUT,
    LENS_PORT_CLK_RELEASE_TIMEOUT,
    LENS_PORT_SPI_BUFFER_TOO_SMALL,
    LENS_PORT_NO_ANSWER_AVAILABLE,

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

    UTILS_TOO_BIT_BIT_OF_BYTE,

    AV_LENS_PORT_UNSET,
    AV_LENS_INITIALIZER_UNSET,

    FOCUS_MOVE_TIMEOUT
};

String errorCodeToString(errorCodes theErrorCode);

#endif //_ERRORS_H
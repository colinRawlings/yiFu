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

    LENS_MAN_BAD_CR_LENGTH,
    LENS_MAN_BAD_CR_RESPONSE,
    LENS_MAN_BAD_FL,

    UI_MSG_BUFFER_EMPTY
};

String errorCodeToString(errorCodes theErrorCode);

#endif //_ERRORS_H
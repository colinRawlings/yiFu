#include "errors.h"

String errorCodeToString(errorCodes theErrorCode)
{
    switch (theErrorCode)
    {
    case SUCCESS:
        return "SUCCESS";

    case LENS_PORT_CLK_PULL_DOWN_TIMEOUT:
        return "LENS_PORT_CLK_PULL_DOWN_TIMEOUT";
    case LENS_PORT_CLK_RELEASE_TIMEOUT:
        return "LENS_PORT_CLK_RELEASE_TIMEOUT";
    case LENS_PORT_SPI_BUFFER_TOO_SMALL:
        return "LENS_PORT_SPI_BUFFER_TOO_SMALL";
    case LENS_PORT_NO_ANSWER_AVAILABLE:
        return "LENS_PORT_NO_ANSWER_AVAILABLE";

    case LENS_INIT_LENS_PORT_UNSET:
        return "LENS_INIT_LENS_PORT_UNSET";
    case LENS_INIT_BAD_CR_LENGTH:
        return "LENS_INIT_BAD_CR_LENGTH";
    case LENS_INIT_BAD_CR_RESPONSE:
        return "LENS_INIT_BAD_CR_RESPONSE";
    case LENS_INIT_BAD_FL:
        return "LENS_INIT_BAD_FL";
    case LENS_INIT_SLOW_CR_FAILED:
        return "LENS_INIT_SLOW_CR_FAILED";

    case UI_MSG_BUFFER_EMPTY:
        return "UI_MSG_BUFFER_EMPTY";
    case UI_LENS_MAN_UNSET:
        return "UI_LENS_MAN_UNSET";
    case UI_SERIAL_PORT_UNSET:
        return "UI_SERIAL_PORT_UNSET";

    case UTILS_TOO_BIT_BIT_OF_BYTE:
        return "UTILS_TOO_BIT_BIT_OF_BYTE";

    case FOCUS_MOVE_TIMEOUT:
        return "FOCUS_MOVE_TIMEOUT";

    case FD_MAN_LENS_PORT_UNSET:
        return "FD_MAN_LENS_PORT_UNSET";

    case FL_MAN_LENS_PORT_UNSET:
        return "FL_MAN_LENS_PORT_UNSET";

    case AV_LENS_PORT_UNSET:
        return "AV_LENS_PORT_UNSET";
    case AV_LENS_INITIALIZER_UNSET:
        return "AV_LENS_INITIALIZER_UNSET";

    default:
        return "UNKNOWN_CODE";
    }
}
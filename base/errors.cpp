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

    case LENS_MAN_BAD_CR_LENGTH:
        return "LENS_MAN_BAD_CR_LENGTH";
    case LENS_MAN_BAD_CR_RESPONSE:
        return "LENS_MAN_BAD_CR_RESPONSE";

    default:
        return "UNKNOWN_CODE";
    }
}
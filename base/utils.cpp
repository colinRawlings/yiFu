#include "utils.h"

#include "errors.h"
#include "Arduino.h"

//-----------------------------------------------------------------
int convertBytesToInt(byte leastSignificantByte, byte mostSignificantByte)
{
    return 256 * ((int)mostSignificantByte) + ((int)leastSignificantByte);
}

//-----------------------------------------------------------------
ErrorCode bitOfByte(byte theByte, int theBit, bool &result)
{
    if (theBit > 7)
    {
        result = false;
        return UTILS_TOO_BIT_BIT_OF_BYTE;
    }

    byte mask = (1 << theBit);
    byte cf = mask & theByte;

    result = cf >> theBit;

    return 0;
}

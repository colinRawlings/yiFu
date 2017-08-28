#include "utils.h"

int convertBytesToInt(byte leastSignificantByte, byte mostSignificantByte)
{
    return 256 * ((int)mostSignificantByte) + ((int)leastSignificantByte);
}
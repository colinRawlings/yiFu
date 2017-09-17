#ifndef _UTILS_H
#define _UTILS_H

#include "errors.h"
#include "Arduino.h"

int convertBytesToInt(byte leastSignificantByte, byte mostSignificantByte);
ErrorCode bitOfByte(byte theByte, int theBit, bool &result);

#endif // _UTILS_H
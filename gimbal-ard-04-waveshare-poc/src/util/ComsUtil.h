#ifndef ComsUtil_h
#define ComsUtil_h

#include <Arduino.h>
#include <CRC.h>

#include "Define.h"

class ComsUtil {
private:
    static CRC8 crc;

public:

    static uint8_t calcCrcOfVector(const vector________t& data);

};

#endif
#include "ComsUtil.h"

CRC8 ComsUtil::crc;

uint8_t ComsUtil::calcCrcOfVector(const vector________t& data) {
    ComsUtil::crc.restart();
    ComsUtil::crc.add((uint8_t*)&data.x, 4);
    ComsUtil::crc.add((uint8_t*)&data.y, 4);
    ComsUtil::crc.add((uint8_t*)&data.z, 4);
    return ComsUtil::crc.calc();
}
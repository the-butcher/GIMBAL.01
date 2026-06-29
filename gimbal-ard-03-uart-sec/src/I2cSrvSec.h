#ifndef I2cSrvSec_h
#define I2cSrvSec_h

#include <Arduino.h>
#include <Wire.h>

#include "Define.h"

class I2cSrvSec {

private:
    static void onReceive(int len);
    static void onRequest();
    static uint64_t packetCount;
    static bool newDataFlag;
    static vector________t lastReadData;

public:

    static bool powerup();
    static bool depower();
    static bool hasNewData();
    static vector________t getLastReadData();
};

#endif
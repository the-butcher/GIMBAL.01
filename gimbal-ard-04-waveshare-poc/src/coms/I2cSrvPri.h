#ifndef I2cSrvPri_h
#define I2cSrvPri_h

#include <Arduino.h>
#include <Wire.h>

#include "Define.h"

class I2cSrvPri {

private:
    static uint64_t packetCount;

public:
    static bool sendData(vector________t sendData);
    static bool powerup();
    static bool depower();

};

#endif
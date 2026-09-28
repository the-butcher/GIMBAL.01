#ifndef UartSrv_h
#define UartSrv_h

#include <Arduino.h>

#include "Define.h"

class UartSrv {

private:
    static HardwareSerial uartSerial;
    static vector________t lastRecvData;
    static bool newRecvDataFlag;


public:

    static uint64_t totalRecvCount;
    static uint64_t firstRecvMillis;
    
    static bool readData();
    static bool powerup();
    static bool depower();

    static bool hasNewRecvData();
    static vector________t getLastRecvData();
};

#endif
#ifndef UartSrv_h
#define UartSrv_h

#include <Arduino.h>

#include "Define.h"
#include "util/ComsUtil.h"

class UartSrv {

private:
    static HardwareSerial uartSerial;
    static vector________t lastRecvData;
    static bool newRecvDataFlag;
    static SemaphoreHandle_t xMutex;


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
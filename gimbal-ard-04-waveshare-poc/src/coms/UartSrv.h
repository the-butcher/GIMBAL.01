#ifndef UartSrv_h
#define UartSrv_h

#include <Arduino.h>

#include "Define.h"

class UartSrv {

private:
    static HardwareSerial uartSerial;

public:
    static bool sendData(vector________t sendData);
    static bool powerup();
    static bool depower();
};

#endif
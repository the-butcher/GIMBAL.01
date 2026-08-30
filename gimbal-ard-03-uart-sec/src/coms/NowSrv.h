#ifndef NowSrv_h
#define NowSrv_h

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "Define.h"

class NowSrv {
private:

public:

    static bool newRecvDataFlag;
    static uint64_t totalRecvCount;
    static uint64_t firstRecvMillis;

    static vector________t lastRecvData;

    static void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);

    static bool powered;
    static bool powerup();
    static bool depower();

    static bool hasNewRecvData();
    static vector________t getLastRecvData();

    static uint64_t getRecvInterval();

};

#endif
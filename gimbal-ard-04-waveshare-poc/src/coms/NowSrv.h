#ifndef NowSrv_h
#define NowSrv_h

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "Define.h"

class NowSrv {
private:

    /**
     * the remote peer info
     */
    static esp_now_peer_info_t peerInfo;

public:

    static bool pndSendDataFlag;
    static uint64_t totalSendCount;
    static uint64_t firstSendMillis;

    static void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status);
    // static void OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);
    static bool sendData(vector________t data);

    static bool powered;
    static bool powerup();
    static bool depower();


};

#endif
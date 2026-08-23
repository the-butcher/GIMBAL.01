#include "NowSrv.h"

esp_now_peer_info_t NowSrv::peerInfo;

bool NowSrv::pndSendDataFlag;
uint64_t NowSrv::totalSendCount = 0;
uint64_t NowSrv::firstSendMillis = 0;

bool NowSrv::powered;

bool NowSrv::powerup() {

    WiFi.mode(WIFI_STA);

    NowSrv::powered = esp_now_init() == ESP_OK;

    if (NowSrv::powered) {

        esp_now_register_send_cb(esp_now_send_cb_t(NowSrv::OnDataSent));

        memcpy(NowSrv::peerInfo.peer_addr, NOW_ADDR_BRDCST, 6);
        NowSrv::peerInfo.channel = 0;
        NowSrv::peerInfo.encrypt = false;

        NowSrv::powered &= esp_now_add_peer(&NowSrv::peerInfo) == ESP_OK;

        // esp_now_register_recv_cb(esp_now_recv_cb_t(NowSrv::OnDataRecv));

    }

    return NowSrv::powered;

}

/**
 * not used currently
 */
bool NowSrv::depower() {

    esp_now_deinit();

    esp_now_unregister_send_cb();
    esp_now_unregister_recv_cb();

    WiFi.mode(WIFI_OFF);

    return true;

}

/**
 * broadcast motor data
 */
bool NowSrv::sendData(vector________t data) {

    if (!NowSrv::pndSendDataFlag) {

        NowSrv::totalSendCount++;
        esp_err_t result = esp_now_send(NOW_ADDR_BRDCST, (uint8_t*)&data, sizeof(data));

        NowSrv::pndSendDataFlag = true;
        if (NowSrv::firstSendMillis == 0) {
            NowSrv::firstSendMillis = millis();
        }
        return result == ESP_OK;

    } else {
        return false;
    }



}

/**
 * data sent callback
 */
void NowSrv::OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
    NowSrv::pndSendDataFlag = false;
}

// /**
//  * data receive callback
//  */
// void NowSrv::OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {



// }

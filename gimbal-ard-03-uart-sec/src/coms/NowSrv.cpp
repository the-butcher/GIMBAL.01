#include "NowSrv.h"

bool NowSrv::powered;
vector________t NowSrv::lastRecvData = { 0, 0, 0 };
bool NowSrv::newRecvDataFlag = false;

uint64_t NowSrv::totalRecvCount = 0;
uint64_t NowSrv::firstRecvMillis = 0;

// Function to convert a struct to a byte array
// https://wokwi.com/projects/384215584338530305
template <typename T>
void serializeData(const T& inputStruct, uint8_t* outputBytes) {
    memcpy(outputBytes, &inputStruct, sizeof(T));
}

// Function to convert a byte array to a struct
// https://wokwi.com/projects/384215584338530305
template <typename T>
void deserializeData(const uint8_t* inputBytes, uint16_t offset, T& outputStruct) {
    memcpy(&outputStruct, inputBytes + offset, sizeof(T));
}

// https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/
bool NowSrv::powerup() {

    WiFi.mode(WIFI_STA);

    NowSrv::powered = esp_now_init() == ESP_OK;

    if (NowSrv::powered) {

        esp_now_register_recv_cb(esp_now_recv_cb_t(NowSrv::OnDataRecv));

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

bool NowSrv::hasNewRecvData() {
    return NowSrv::newRecvDataFlag;
}

vector________t NowSrv::getLastRecvData() {
    NowSrv::newRecvDataFlag = false;
    return NowSrv::lastRecvData;
}

/**
 * data receive callback
 */
void NowSrv::OnDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {

    if (len == sizeof(vector________t)) { // incoming, neither bitmaps nor ledbar

        NowSrv::totalRecvCount++;
        memcpy(&NowSrv::lastRecvData, incomingData, sizeof(vector________t));
        NowSrv::newRecvDataFlag = true;
        if (NowSrv::firstRecvMillis == 0) {
            NowSrv::firstRecvMillis = millis();
        }

    }

}

uint64_t NowSrv::getRecvInterval() {
    // TODO as of actual receive interval
    return 10L;
}

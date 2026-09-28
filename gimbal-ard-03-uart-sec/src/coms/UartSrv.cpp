#include "UartSrv.h"

HardwareSerial UartSrv::uartSerial = HardwareSerial(1);
vector________t UartSrv::lastRecvData = { 0, 0, 0 };
bool UartSrv::newRecvDataFlag = false;

uint64_t UartSrv::totalRecvCount = 0;
uint64_t UartSrv::firstRecvMillis = 0;

// https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/
bool UartSrv::powerup() {

    UartSrv::uartSerial.begin(UART_BAUD_RATE, SERIAL_8N1, GPIO_NUM_SEC_TX, GPIO_NUM_SEC_RX); // crossing RX / TX on purpose

    return true;

}

/**
 * not used currently
 */
bool UartSrv::depower() {
    return true;
}

bool UartSrv::hasNewRecvData() {
    return UartSrv::newRecvDataFlag;
}

vector________t UartSrv::getLastRecvData() {
    UartSrv::newRecvDataFlag = false;
    return UartSrv::lastRecvData;
}

bool UartSrv::readData() {

    int available = UartSrv::uartSerial.available();
    if (available > sizeof(vector________t)) {

        uint8_t* bytes = new uint8_t[sizeof(vector________t)];
        size_t bytesRead = UartSrv::uartSerial.readBytesUntil(0x0A, bytes, sizeof(vector________t));
        if (bytesRead == sizeof(vector________t)) {

            UartSrv::totalRecvCount++;
            memcpy(&UartSrv::lastRecvData, bytes, sizeof(vector________t));
            UartSrv::newRecvDataFlag = true;
            if (UartSrv::firstRecvMillis == 0) {
                UartSrv::firstRecvMillis = millis();
            }
            return true;

        }

    }
    return false;

}
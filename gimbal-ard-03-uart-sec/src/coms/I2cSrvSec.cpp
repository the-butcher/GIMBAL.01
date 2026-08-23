#include "I2cSrvSec.h"

vector________t I2cSrvSec::lastReadData = { 0, 0, 0 };
uint64_t I2cSrvSec::packetCount = 0;
bool I2cSrvSec::newDataFlag = false;

bool I2cSrvSec::powerup() {

    Wire1.onReceive(I2cSrvSec::onReceive);
    Wire1.onRequest(I2cSrvSec::onRequest);
    Wire1.begin(I2C_SRV_SEC_ADDRESS, GPIO_NUM_I2C_SEC_SDA, GPIO_NUM_I2C_SEC_SCL, 0);

    return true;

}

/**
 * not used currently
 */
bool I2cSrvSec::depower() {

    return true;

}

bool I2cSrvSec::hasNewData() {
    return I2cSrvSec::newDataFlag;
}

vector________t I2cSrvSec::getLastReadData() {
    I2cSrvSec::newDataFlag = false;
    return I2cSrvSec::lastReadData;
}


void I2cSrvSec::onReceive(int len) {

    int available = Wire1.available();
    if (available >= sizeof(vector________t)) {
        uint8_t* bytes = new uint8_t[sizeof(vector________t)];
        size_t bytesRead = Wire1.readBytes(bytes, sizeof(vector________t)); // UartSrv::uartSerial.readBytesUntil(0x0A, bytes, sizeof(vector________t));
        if (bytesRead == sizeof(vector________t)) {
            memcpy(&I2cSrvSec::lastReadData, bytes, sizeof(vector________t));
            I2cSrvSec::newDataFlag = true;
        }
    }

    // Serial.printf("onReceive[%d]: ", len);
    // while (Wire1.available()) {
    //     Serial.write(Wire1.read());
    // }
    // Serial.println();

}

void I2cSrvSec::onRequest() {

    Wire1.print(I2cSrvSec::packetCount++);
    Wire1.print(" Packets.");
    Serial.println("onRequest");
    Serial.println();

}

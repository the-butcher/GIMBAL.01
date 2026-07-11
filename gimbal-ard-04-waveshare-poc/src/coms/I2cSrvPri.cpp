#include "I2cSrvPri.h"

uint64_t I2cSrvPri::packetCount = 0;

bool I2cSrvPri::powerup() {

    // Wire1 will have begun by this time

    return true;

}

/**
 * not used currently
 */
bool I2cSrvPri::depower() {

    // nothing
    return true;

}

bool I2cSrvPri::sendData(vector________t sendData) {

    // Write message to the slave
    Wire1.beginTransmission(I2C_ADDR__MOT_Y);
    Wire1.write((uint8_t*)&sendData, sizeof(vector________t));
    // Wire1.printf("Hello World! %lu", I2cSrvPri::packetCount++);
    uint8_t error = Wire1.endTransmission(true);
    // Serial.printf("endTransmission: %u\n", error);

    // // Read 16 bytes from the slave
    // uint8_t bytesReceived = Wire1.requestFrom(I2C_ADDR__MOT_Y, 16);

    // Serial.printf("requestFrom: %u\n", bytesReceived);
    // if ((bool)bytesReceived) {  //If received more than zero bytes
    //     uint8_t temp[bytesReceived];
    //     Wire1.readBytes(temp, bytesReceived);
    //     log_print_buf(temp, bytesReceived);
    // }

    return true;
}

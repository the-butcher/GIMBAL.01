#include "UartSrv.h"

HardwareSerial UartSrv::uartSerial = HardwareSerial(1);

// https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/
bool UartSrv::powerup() {

    UartSrv::uartSerial.begin(UART_BAUD_RATE, SERIAL_8N1, GPIO_NUM_PRI_RX, GPIO_NUM_PRI_TX); // c

    return true;

}

/**
 * not used currently
 */
bool UartSrv::depower() {
    return true;
}

bool UartSrv::sendData(vector________t sendData) {
    sendData.crc = ComsUtil::calcCrcOfVector(sendData);
    UartSrv::uartSerial.write((uint8_t*)&sendData, sizeof(vector________t));
    UartSrv::uartSerial.write(0x0A); // end of line
    return true;
}

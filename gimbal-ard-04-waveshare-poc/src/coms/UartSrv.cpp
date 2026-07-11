#include "UartSrv.h"

HardwareSerial UartSrv::uartSerial = HardwareSerial(1);
vector________t sendData = { 0.1, 0.2, 0.3 };

// https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/
bool UartSrv::powerup() {

    UartSrv::uartSerial.begin(UART_BAUD_RATE, SERIAL_8N1, GPIO_NUM_PRI_RX, GPIO_NUM_PRI_TX); // 38400

    return true;

}

/**
 * not used currently
 */
bool UartSrv::depower() {

    return true;

}

bool UartSrv::sendData(vector________t sendData) {
    UartSrv::uartSerial.write((uint8_t*)&sendData, sizeof(vector________t));
    UartSrv::uartSerial.write(0x0A); // end of line
    return true;
}

bool UartSrv::readData() {

    int available = UartSrv::uartSerial.available();
    Serial.print("available: ");
    Serial.println(available);
    if (available > sizeof(vector________t)) {
        uint8_t* bytes = new uint8_t[sizeof(vector________t)];
        UartSrv::uartSerial.read(bytes, sizeof(vector________t));
        memcpy(&UartSrv::lastReadData, bytes, sizeof(vector________t));
        return true;
    } else {
        return false;
    }

}

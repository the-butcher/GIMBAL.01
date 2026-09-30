#include "UartSrv.h"

HardwareSerial UartSrv::uartSerial = HardwareSerial(1);
vector________t UartSrv::lastRecvData = { 0, 0, 0 };
bool UartSrv::newRecvDataFlag = false;

uint64_t UartSrv::totalRecvCount = 0;
uint64_t UartSrv::firstRecvMillis = 0;

SemaphoreHandle_t UartSrv::xMutex = NULL;

// https://randomnerdtutorials.com/esp-now-two-way-communication-esp32/
bool UartSrv::powerup() {

    UartSrv::uartSerial.begin(UART_BAUD_RATE, SERIAL_8N1, GPIO_NUM_SEC_TX, GPIO_NUM_SEC_RX); // crossing RX / TX on purpose
    UartSrv::xMutex = xSemaphoreCreateMutex();

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
    vector________t lastRecvCopy;
    if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
        lastRecvCopy = UartSrv::lastRecvData;
        // if (abs(lastRecvCopy.z) < 0.0001f) {
        //     Serial.println("copy z is zero");
        // }
        xSemaphoreGive(xMutex);
    }
    return lastRecvCopy;
}

bool UartSrv::readData() {

    int available = UartSrv::uartSerial.available();
    if (available > SIZE_OF_VECTOR_T) {
        
        uint8_t* bytes = new uint8_t[SIZE_OF_VECTOR_T];
        size_t bytesRead = UartSrv::uartSerial.readBytesUntil(0x0A, bytes, SIZE_OF_VECTOR_T);
        bool isValid = (bytesRead == SIZE_OF_VECTOR_T);
        if (isValid) {

            // serialize the bytes into a vector________t struct and calculate the CRC
            vector________t currRecvData;
            memcpy(&currRecvData, bytes, SIZE_OF_VECTOR_T);
            uint8_t crc = ComsUtil::calcCrcOfVector(currRecvData);
            isValid = (crc == currRecvData.crc);

            if (isValid) {
                UartSrv::totalRecvCount++;
                if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
                    UartSrv::lastRecvData = currRecvData;
                    // if (abs(UartSrv::lastRecvData.z) < 0.0001f) {
                    //     Serial.print("last z is zero, x: ");
                    //     Serial.print(UartSrv::lastRecvData.x, 5);
                    //     Serial.print(", y: ");
                    //     Serial.print(UartSrv::lastRecvData.y, 5);
                    //     Serial.print(", z: ");
                    //     Serial.print(UartSrv::lastRecvData.z, 5);   
                    //     Serial.print(", crcA: 0x");
                    //     Serial.print(crc, HEX);
                    //     Serial.print(", crcB: 0x");
                    //     Serial.println(UartSrv::lastRecvData.crc, HEX);
                    // }
                    xSemaphoreGive(xMutex);
                }
                UartSrv::newRecvDataFlag = true;
                if (UartSrv::firstRecvMillis == 0) {
                    UartSrv::firstRecvMillis = millis();
                }
            } else {
                Serial.print("crcA: ");
                Serial.println(crc, HEX);
                Serial.print("x: ");
                Serial.println(String(currRecvData.x, 3));
                Serial.print("crcB: ");
                Serial.println(currRecvData.crc, HEX);
            }

        } 

        UartSrv::uartSerial.flush();
        delete[] bytes;

        return isValid;

    }
    return false;

}
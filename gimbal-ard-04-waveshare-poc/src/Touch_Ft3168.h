#ifndef Touch_Ft3168_h
#define Touch_Ft3168_h

#include <Arduino.h>
#include "driver/i2c.h"
#include "Define.h"


class Touch_Ft3168 {
private:

    static uint8_t writeBuffer(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len);
    static uint8_t readBuffer(uint8_t addr, uint8_t reg, uint8_t* buf, uint8_t len);
    // static uint8_t readWriteDevice(uint8_t addr, uint8_t* writeBuf, uint8_t writeLen, uint8_t* readBuf, uint8_t readLen);

public:

    static void touchBegin(void);
    static uint8_t getTouch(uint16_t* x, uint16_t* y);

};

#endif
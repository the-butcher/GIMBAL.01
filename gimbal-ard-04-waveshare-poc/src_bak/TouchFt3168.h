#ifndef TouchFt3168_H
#define TouchFt3168_H

#include "driver/i2c.h"
#include "define.h"

#ifdef __cplusplus
extern "C" {
#endif

    void touchBegin(void);
    uint8_t getTouch(uint16_t* x, uint16_t* y);

#ifdef __cplusplus
}
#endif
#endif
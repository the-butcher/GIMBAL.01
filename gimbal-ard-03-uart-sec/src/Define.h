#ifndef Define_h
#define Define_h

#include <Arduino.h>

#define USE____SEEED false
#define USE_____QTPY true

// SEEED        ┌────┐
//          ┌───┤    ├───┐
//  SDA1 01 │O  └────┘  O│    5V
//  SCL1 02 │O          O│    GND
//    CS 03 │O          O│    3V3
//   FLT 04 │O          O│ 09 MO
//    M1 05 │O          O│ 08 MI
//    M2 06 │O          O│ 07 SCK
//    M3 43 │O          O│ 44 EN
//          └────────────┘

// QT PY        ┌────┐
//          ┌───┤    ├───┐
//  SDA1 18 │O  └────┘  O│    5V
//  SCL1 17 │O          O│    GND
//    CS 09 │O          O│    3V3
//   FLT 08 │O          O│ 35 MO
//    M1 07 │O          O│ 37 MI
//    M2 06 │O          O│ 36 SCK
//    M3 05 │O          O│ 16 EN
//          └────────────┘

#if USE____SEEED == true

const gpio_num_t GPIO_NUM_ENC_CS = GPIO_NUM_3;

const gpio_num_t GPIO_NUM_MOT_M1 = GPIO_NUM_5;
const gpio_num_t GPIO_NUM_MOT_M2 = GPIO_NUM_6;
const gpio_num_t GPIO_NUM_MOT_M3 = GPIO_NUM_43;
const gpio_num_t GPIO_NUM_MOT_EN = GPIO_NUM_44;

const gpio_num_t GPIO_NUM_SEC_RX = GPIO_NUM_2;
const gpio_num_t GPIO_NUM_SEC_TX = GPIO_NUM_1;

const uint32_t UART_BAUD_RATE = 115200;

const gpio_num_t GPIO_NUM_I2C_SEC_SDA = GPIO_NUM_1;
const gpio_num_t GPIO_NUM_I2C_SEC_SCL = GPIO_NUM_2;

#endif

#if USE_____QTPY == true

const gpio_num_t GPIO_NUM_ENC_CS = GPIO_NUM_8;
const gpio_num_t GPIO_NUM_MOT_M1 = GPIO_NUM_7;
const gpio_num_t GPIO_NUM_MOT_M2 = GPIO_NUM_6;
const gpio_num_t GPIO_NUM_MOT_M3 = GPIO_NUM_5;
const gpio_num_t GPIO_NUM_MOT_EN = GPIO_NUM_16;

const gpio_num_t GPIO_NUM_SEC_RX = GPIO_NUM_17; // A1, second from usb-c end
const gpio_num_t GPIO_NUM_SEC_TX = GPIO_NUM_18; // A0, first from usb-c end
const uint32_t UART_BAUD_RATE = 38400;

const gpio_num_t GPIO_NUM_I2C_SEC_SDA = GPIO_NUM_18;
const gpio_num_t GPIO_NUM_I2C_SEC_SCL = GPIO_NUM_17;

#endif

const uint8_t REGRESSION____ORDER = 2;
const uint8_t REGRESSION_NUM_VALS = 32;

const uint8_t I2C_SRV_SEC_ADDRESS = 0x48; // 72

typedef struct {
    float x;
    float y;
    float z;
} vector________t;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} quaternion____t;

#endif
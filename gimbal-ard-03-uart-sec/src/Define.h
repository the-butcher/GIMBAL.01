#ifndef Define_h
#define Define_h

#include <Arduino.h>

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
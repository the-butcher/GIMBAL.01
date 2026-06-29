#ifndef Define_h
#define Define_h

#include <Arduino.h>

/**
 * primary (Wire) I2C, touch and amoled
 */
const gpio_num_t GPIO_NUM___SDA0 = GPIO_NUM_47;
const gpio_num_t GPIO_NUM___SCL0 = GPIO_NUM_48;

/**
 * secondary (Wire1) I2C, orientation and rotary encoder
 */
const gpio_num_t GPIO_NUM___SDA1 = GPIO_NUM_15;
const gpio_num_t GPIO_NUM___SCL1 = GPIO_NUM_16;

const gpio_num_t GPIO_NUM____SW0 = GPIO_NUM_44;
const gpio_num_t GPIO_NUM____SW1 = GPIO_NUM_43;
const gpio_num_t GPIO_NUM____SW2 = GPIO_NUM_45;

/**
 * deprecated
 */
const gpio_num_t GPIO_NUM_PRI_RX = GPIO_NUM_48;
const gpio_num_t GPIO_NUM_PRI_TX = GPIO_NUM_43;

const uint32_t UART_BAUD_RATE = 115200;

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
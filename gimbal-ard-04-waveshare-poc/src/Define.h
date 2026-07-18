#ifndef Define_h
#define Define_h

#include <Arduino.h>

#define USE____CANON true

/**
 * primary (Wire) I2C, touch display
 */
const gpio_num_t GPIO_NUM___SDA0 = GPIO_NUM_47;
const gpio_num_t GPIO_NUM___SCL0 = GPIO_NUM_48;

/**
 * secondary (Wire1) I2C, orientation and rotary encoder, motors
 */
const gpio_num_t GPIO_NUM___SDA1 = GPIO_NUM_6;
const gpio_num_t GPIO_NUM___SCL1 = GPIO_NUM_5;

const gpio_num_t GPIO_NUM____SW0 = GPIO_NUM_44;
const gpio_num_t GPIO_NUM____SW1 = GPIO_NUM_43;
const gpio_num_t GPIO_NUM____SW2 = GPIO_NUM_45;

/**
 * deprecated
 */
const gpio_num_t GPIO_NUM_PRI_RX = GPIO_NUM_48;
const gpio_num_t GPIO_NUM_PRI_TX = GPIO_NUM_43;
const uint32_t   UART_BAUD_RATE = 115200;

// TODO :: there must be multiple addresses for the three motors
const uint8_t    I2C_ADDR__MOT_Y = 0x48; // 72

// touch display dimensions
const uint16_t   TD_DIM________X = 280;
const uint16_t   TD_DIM________Y = 456;
const uint8_t    TD_BITS_P_PIXEL = 16;

// display pins
const gpio_num_t GPIO_NUM_TD__CS = GPIO_NUM_9;  // touch display cs
const gpio_num_t GPIO_NUM_TD_CLK = GPIO_NUM_10; // touch display clk
const gpio_num_t GPIO_NUM_TD_DT0 = GPIO_NUM_11; // touch display data0
const gpio_num_t GPIO_NUM_TD_DT1 = GPIO_NUM_12; // touch display data1
const gpio_num_t GPIO_NUM_TD_DT2 = GPIO_NUM_13; // touch display data2
const gpio_num_t GPIO_NUM_TD_DT3 = GPIO_NUM_14; // touch display data3
const gpio_num_t GPIO_NUM_TD_RST = GPIO_NUM_21; // touch display rst

// lvgl values
const uint16_t   TD_BUF___HEIGHT = TD_DIM________Y / 4; // 114
const uint8_t    TD_TICK______MS = 2;
const uint16_t   TD_DELAY_MAX_MS = 500;         // LVGL Indicates the maximum time for a task to run
const uint8_t    TD_DELAY_MIN_MS = 1;           // LVGL Minimum time to run a task
const uint16_t   TD_TASK___STACK = 4 * 1024;    // LVGL runs the task stack
const uint8_t    TD_TASK____PRIO = 2;           // LVGL Running task priority

// I2C address of touch display
const uint8_t    I2C_ADDR__TOUCH = 0x38;

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

typedef struct {
    char ssid[64];
    char pass[64];
} network_t;

const network_t NETWORK_INVAL = {
    "",
    ""
};

#if USE____CANON == true
const network_t NETWORK_CCAPI = {
    "EOSR100_CCEE7E-583_Canon0A",
    "***"
};
const String CCAPI______BASE_URL = "http://192.168.1.2:8080/ccapi"; // Canon Eos R100 AP
#else
const network_t NETWORK_CCAPI = {
    "the-butchers",
    "***"
};
const String CCAPI______BASE_URL = "http://192.168.0.38:1880/endpoint"; // Node-Red in Homeassistant
#endif

const String CCAPI__________ROOT = "/";
const String CCAPI_____MODE_DIAL = "/ver100/shooting/settings/shootingmodedial";
const String CCAPI_SHUTTERBUTTON = "/ver100/shooting/control/shutterbutton";

#endif
#ifndef SensorRotEnc_h
#define SensorRotEnc_h

#include <Adafruit_seesaw.h>
#include <seesaw_neopixel.h>
#include <Arduino.h>
#include <Wire.h>

#include "Define.h"

#define SEESAW__SWITCH 24
#define SEESAW__NEOPIX 6
#define SEESAW____ADDR 0x36

class SensorRotEnc {
private:

    static Adafruit_seesaw baseSensor;
    static seesaw_NeoPixel basePixel;
    static int32_t position;

public:

    static bool read();

    static bool powered;
    static bool powerup();
    static bool depower();
    static int32_t getPosition();

};

#endif
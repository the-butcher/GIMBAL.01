#ifndef SignalUtil_h
#define SignalUtil_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "Define.h"

class SignalUtil {
private:
    static Adafruit_NeoPixel basePixel;

public:

    static bool powered;
    static bool powerup();
    static bool depower();

    static void setBrightness(uint8_t brightness);
    static void setColor(uint8_t r, uint8_t g, uint8_t b);

};

#endif
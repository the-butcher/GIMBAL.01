#include "SignalUtil.h"

bool SignalUtil::powered = false;
Adafruit_NeoPixel SignalUtil::basePixel(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);      

bool SignalUtil::powerup() {
    pinMode(NEOPIXEL_POWER, OUTPUT);
    digitalWrite(NEOPIXEL_POWER, HIGH);
    SignalUtil::basePixel.begin();
    SignalUtil::basePixel.setBrightness(10);
    powered = true;
    return true;
}

bool SignalUtil::depower() {
    powered = false;
    return true;
}

void SignalUtil::setBrightness(uint8_t brightness) {
    SignalUtil::basePixel.setBrightness(brightness);
}

void SignalUtil::setColor(uint8_t r, uint8_t g, uint8_t b) {
    SignalUtil::basePixel.setPixelColor(0, r, g, b);  
    SignalUtil::basePixel.show(); 
}
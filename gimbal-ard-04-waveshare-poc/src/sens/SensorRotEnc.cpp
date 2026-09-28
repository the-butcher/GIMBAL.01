#include "SensorRotEnc.h"

Adafruit_seesaw SensorRotEnc::baseSensor(&Wire1);
seesaw_NeoPixel SensorRotEnc::basePixel = seesaw_NeoPixel(1, SEESAW__NEOPIX, NEO_GRB + NEO_KHZ800, &Wire1);

int32_t SensorRotEnc::position = 0;

bool SensorRotEnc::powered = false;

bool SensorRotEnc::powerup() {

    SensorRotEnc::powered = SensorRotEnc::baseSensor.begin(SEESAW____ADDR);
    SensorRotEnc::powered = SensorRotEnc::powered && SensorRotEnc::basePixel.begin(SEESAW____ADDR);

    SensorRotEnc::basePixel.setBrightness(3);
    SensorRotEnc::basePixel.setPixelColor(0, 255, 0, 0); // Red
    SensorRotEnc::basePixel.show();

    // use a pin for the built in encoder switch
    SensorRotEnc::baseSensor.pinMode(SEESAW__SWITCH, INPUT_PULLUP);

    // get starting position
    SensorRotEnc::position = SensorRotEnc::baseSensor.getEncoderPosition();

    delay(10);
    SensorRotEnc::baseSensor.setGPIOInterrupts((uint32_t)1 << SEESAW__SWITCH, 1);
    SensorRotEnc::baseSensor.enableEncoderInterrupt();

    return SensorRotEnc::powered;

}

bool SensorRotEnc::depower() {
    // nothing
    return true;
}

bool SensorRotEnc::read() {
    SensorRotEnc::position = SensorRotEnc::baseSensor.getEncoderPosition();
    return true;
}

int32_t SensorRotEnc::getPosition() {
    return SensorRotEnc::position;
}

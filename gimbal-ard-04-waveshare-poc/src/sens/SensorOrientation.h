#ifndef SensorOrientation_h
#define SensorOrientation_h

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>

#include "Define.h"

/**
 * orientation.x has axis normal to sensor plane
 * orientation.y has axis on sensor plane, perpendicular to cable
 * orientation.z has axis sensor plane, along cable
 */
class SensorOrientation {
private:

    static Adafruit_BNO055 baseSensor;
    static vector________t orientation;
    static quaternion____t quaternion;
    static float GRAD_TO_RAD;

public:

    static bool read();
    static vector________t getOrientation();
    static quaternion____t getQuaternion();

    static bool powered;
    static bool powerup();
    static bool depower();

};

#endif
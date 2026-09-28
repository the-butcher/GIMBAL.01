#ifndef SensorBno085_h
#define SensorBno085_h

#include <Adafruit_BNO08x.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>

#include "Define.h"

class SensorBno085 {
private:

    static Adafruit_BNO08x baseSensor;
    static vector________t orientation;
    static quaternion____t quaternion;
    static void quaternionToEuler(quaternion____t quat, vector________t* ypr, bool degrees = false);
    static bool enableReports();
    static float GRAD_TO_RAD;

public:

    static uint64_t totalReadCount;
    static uint64_t firstReadMillis;

    static bool read();
    static vector________t getOrientation();
    static quaternion____t getQuaternion();

    static bool powered;
    static bool powerup();
    static bool depower();

};

#endif
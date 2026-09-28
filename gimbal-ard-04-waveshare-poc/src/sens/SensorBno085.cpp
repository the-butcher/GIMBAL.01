#include "SensorBno085.h"

Adafruit_BNO08x SensorBno085::baseSensor(-1);
vector________t SensorBno085::orientation = { 0, 0, 0 };
quaternion____t SensorBno085::quaternion = { 0, 0, 0, 0 };
uint64_t SensorBno085::totalReadCount = 0;
uint64_t SensorBno085::firstReadMillis = 0;

bool SensorBno085::powered = false;
float SensorBno085::GRAD_TO_RAD = PI / 180.0;

bool SensorBno085::powerup() {
    SensorBno085::powered = SensorBno085::baseSensor.begin_I2C(BNO08x_I2CADDR_DEFAULT, &Wire1);
    SensorBno085::powered = SensorBno085::powered && SensorBno085::enableReports();
    return SensorBno085::powered;
}

bool SensorBno085::depower() {
    // nothing
    return true;
}

bool SensorBno085::enableReports() {
  return SensorBno085::baseSensor.enableReport(SH2_GAME_ROTATION_VECTOR, 2500); // 2500us = 2.5ms = 400Hz
}

bool SensorBno085::read() {

    if (SensorBno085::baseSensor.wasReset()) {
        SensorBno085::enableReports();
    }

    sh2_SensorValue_t sensorValue;
    if (SensorBno085::baseSensor.getSensorEvent(&sensorValue)) {

        SensorBno085::totalReadCount++;
        if (SensorBno085::firstReadMillis == 0) {
            SensorBno085::firstReadMillis = millis();
        }

    //   Serial.print("Game Rotation Vector - r: ");
    //   Serial.print(sensorValue.un.gameRotationVector.real);
    //   Serial.print(" i: ");
    //   Serial.print(sensorValue.un.gameRotationVector.i);
    //   Serial.print(" j: ");
    //   Serial.print(sensorValue.un.gameRotationVector.j);
    //   Serial.print(" k: ");
    //   Serial.println(sensorValue.un.gameRotationVector.k);

        SensorBno085::quaternion.w = sensorValue.un.gameRotationVector.real;
        SensorBno085::quaternion.x = sensorValue.un.gameRotationVector.i;   
        SensorBno085::quaternion.y = sensorValue.un.gameRotationVector.j;
        SensorBno085::quaternion.z = sensorValue.un.gameRotationVector.k;

        SensorBno085::quaternionToEuler(SensorBno085::quaternion, &SensorBno085::orientation, false);

        return true;

    } else {
        return false;
    }
    
}

void SensorBno085::quaternionToEuler(quaternion____t quat, vector________t* ypr, bool degrees) {

    float sqr = sq(quat.w);
    float sqi = sq(quat.x);
    float sqj = sq(quat.y);
    float sqk = sq(quat.z);

    ypr->x = atan2(2.0 * (quat.x * quat.y + quat.z * quat.w), (sqi - sqj - sqk + sqr));
    ypr->y = asin(-2.0 * (quat.x * quat.z - quat.y * quat.w) / (sqi + sqj + sqk + sqr));
    ypr->z = atan2(2.0 * (quat.y * quat.z + quat.x * quat.w), (-sqi - sqj + sqk + sqr));

    if (degrees) {
      ypr->x *= RAD_TO_DEG;
      ypr->y *= RAD_TO_DEG;
      ypr->z *= RAD_TO_DEG;
    }

}

vector________t SensorBno085::getOrientation() {
    return SensorBno085::orientation;
}

quaternion____t SensorBno085::getQuaternion() {
    return SensorBno085::quaternion;
}
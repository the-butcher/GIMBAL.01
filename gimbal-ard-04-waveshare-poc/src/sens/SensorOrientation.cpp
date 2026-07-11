#include "SensorOrientation.h"

Adafruit_BNO055 SensorOrientation::baseSensor(55, 0x28, &Wire1);
vector________t SensorOrientation::orientation = { 0, 0, 0 };
quaternion____t SensorOrientation::quaternion = { 0, 0, 0, 0 };

bool SensorOrientation::powered = false;
float SensorOrientation::GRAD_TO_RAD = PI / 180.0;

bool SensorOrientation::powerup() {
    SensorOrientation::powered = SensorOrientation::baseSensor.begin();
    return SensorOrientation::powered;
}

bool SensorOrientation::depower() {
    // nothing
    return true;
}

bool SensorOrientation::read() {

    sensors_event_t orientationData;
    SensorOrientation::baseSensor.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    SensorOrientation::orientation = { orientationData.orientation.x * SensorOrientation::GRAD_TO_RAD, orientationData.orientation.y * SensorOrientation::GRAD_TO_RAD, orientationData.orientation.z * SensorOrientation::GRAD_TO_RAD };

    imu::Quaternion quat = SensorOrientation::baseSensor.getQuat();
    SensorOrientation::quaternion = { (float)quat.x(), (float)quat.y(), (float)quat.z(), (float)quat.w() };

    // Serial.printf("{\"x\":%s,\"y\":%s,\"z\":%s,\"w\":%s}\n", String(Orientation::quaternion.x, 6), String(Orientation::quaternion.y, 6), String(Orientation::quaternion.z, 6), String(Orientation::quaternion.w, 6));

    return true;

}

vector________t SensorOrientation::getOrientation() {
    return SensorOrientation::orientation;
}

quaternion____t SensorOrientation::getQuaternion() {
    return SensorOrientation::quaternion;
}
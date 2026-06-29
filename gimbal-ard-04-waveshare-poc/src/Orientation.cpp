#include "Orientation.h"

Adafruit_BNO055 Orientation::baseSensor(55, 0x28, &Wire1);
vector________t Orientation::orientation = { 0, 0, 0 };
quaternion____t Orientation::quaternion = { 0, 0, 0, 0 };

bool Orientation::powered = false;
float Orientation::GRAD_TO_RAD = PI / 180.0;

bool Orientation::powerup() {
    Orientation::powered = Orientation::baseSensor.begin();
    return Orientation::powered;
}

bool Orientation::depower() {
    // nothing
    return true;
}

bool Orientation::read() {

    sensors_event_t orientationData;
    Orientation::baseSensor.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    Orientation::orientation = { orientationData.orientation.x * Orientation::GRAD_TO_RAD, orientationData.orientation.y * Orientation::GRAD_TO_RAD, orientationData.orientation.z * Orientation::GRAD_TO_RAD };

    imu::Quaternion quat = Orientation::baseSensor.getQuat();
    Orientation::quaternion = { (float)quat.x(), (float)quat.y(), (float)quat.z(), (float)quat.w() };

    // Serial.printf("{\"x\":%s,\"y\":%s,\"z\":%s,\"w\":%s}\n", String(Orientation::quaternion.x, 6), String(Orientation::quaternion.y, 6), String(Orientation::quaternion.z, 6), String(Orientation::quaternion.w, 6));

    return true;

}

vector________t Orientation::getOrientation() {
    return Orientation::orientation;
}

quaternion____t Orientation::getQuaternion() {
    return Orientation::quaternion;
}
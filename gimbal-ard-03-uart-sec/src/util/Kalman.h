#ifndef Kalman_h
#define Kalman_h

#include <Arduino.h>

#include "Define.h"

class Kalman {
private:
    static float dt_;
    static float F_[3][3];
    static float Q_[3][3];
    static float P_[3][3];
    static float H_[3];
    static float R_;
    static float x_[3];
    static bool  initialized_;


public:

    /**
     * the last time (milliseconds) a value was stored
     */
    static uint64_t lastValueMillis;

    static void begin(float dt, float processNoise, float measNoiseStd);
    static void update(float measurement);
    static float predict(float stepsAhead);
    static float position();
    static float velocity();
    static float acceleration();

};

#endif
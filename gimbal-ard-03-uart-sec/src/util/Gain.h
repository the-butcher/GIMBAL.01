#ifndef Gain_h
#define Gain_h

#include <Arduino.h>

#include "Define.h"

// https://www.desmos.com/calculator/9hrbftesvg
class Gain {
private:
    float g1; // max gain, command "F"
    float g2; // min gain, command "f"
    float p; // power
    float o; // helper value
    float m1; // max gain cutoff, a value beyond will yield maxGain
    float m2; // helper value
    void recalculate();


public:

    Gain(float g1, float g2, float m1, float p);

    void setG1(float g1);
    void setG2(float g2);
    void setM1(float m1);
    float getGain(float value);

};

#endif
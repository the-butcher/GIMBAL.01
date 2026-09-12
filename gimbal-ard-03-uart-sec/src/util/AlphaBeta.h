#ifndef AlphaBeta_h
#define AlphaBeta_h

#include <Arduino.h>

#include "Define.h"

class AlphaBeta {
private:
    float _alpha, _beta, _gamma;
    bool _useAccel;
    float _x, _v, _a;
    bool _initialized;


public:

    AlphaBeta(float alpha, float beta, float gamma = 0.0f, bool useAccel = false);

    void init(float initialAngle);
    float update(float measurement, float dt);
    float predict(float dt_future) const;

    float getAngle() const;
    float getVelocity() const;
    float getAcceleration() const;

    void setGains(float alpha, float beta, float gamma = 0.0f);

    void reset();

};

#endif
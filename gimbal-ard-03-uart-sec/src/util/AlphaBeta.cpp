#include "AlphaBeta.h"

AlphaBeta::AlphaBeta(float alpha, float beta, float gamma, bool useAccel)
    : _alpha(alpha), _beta(beta), _gamma(gamma), _useAccel(useAccel),
    _x(0), _v(0), _a(0), _initialized(false) {
}

// Call once with a known starting angle before the control loop begins,
    // otherwise the filter will "swing in" from zero over the first samples.
void AlphaBeta::init(float initialAngle) {
    _x = initialAngle;
    _v = 0.0f;
    _a = 0.0f;
    _initialized = true;
}

// measurement: raw angle reading (rad or deg, be consistent throughout)
// dt: time since last update, in seconds
// returns: filtered angle
float AlphaBeta::update(float measurement, float dt) {
    if (!_initialized) {
        init(measurement);
        return _x;
    }
    if (dt <= 0.0f) return _x;  // guard against bad timestamps

    // --- Predict step ---
    float x_pred = _x + _v * dt + (_useAccel ? 0.5f * _a * dt * dt : 0.0f);
    float v_pred = _v + (_useAccel ? _a * dt : 0.0f);
    float a_pred = _a;

    // --- Correct step ---
    float residual = measurement - x_pred;

    _x = x_pred + _alpha * residual;
    _v = v_pred + (_beta / dt) * residual;

    if (_useAccel) {
        _a = a_pred + (2.0f * _gamma / (dt * dt)) * residual;
    }

    return _x;
}

// Extrapolate the current filtered state forward by latency dt_future
// (this is the "predict Δt ahead" step from the earlier discussion)
float AlphaBeta::predict(float dt_future) const {
    return _x + _v * dt_future
        + (_useAccel ? 0.5f * _a * dt_future * dt_future : 0.0f);
}

float AlphaBeta::getAngle()        const { return _x; }
float AlphaBeta::getVelocity()     const { return _v; }
float AlphaBeta::getAcceleration() const { return _a; }

void AlphaBeta::setGains(float alpha, float beta, float gamma) {
    _alpha = alpha;
    _beta = beta;
    _gamma = gamma;
}

void AlphaBeta::reset() { _initialized = false; }
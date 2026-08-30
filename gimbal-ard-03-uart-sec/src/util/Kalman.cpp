#include "Kalman.h"

float Kalman::dt_;
float Kalman::F_[3][3];
float Kalman::Q_[3][3];
float Kalman::P_[3][3];
float Kalman::H_[3];
float Kalman::R_;
float Kalman::x_[3];
bool  Kalman::initialized_;
uint64_t Kalman::lastValueMillis = 0;

void Kalman::begin(float dt, float processNoise, float measNoiseStd) {

    dt_ = dt;

    // State transition matrix F (constant acceleration model)
    F_[0][0] = 1; F_[0][1] = dt;        F_[0][2] = 0.5f * dt * dt;
    F_[1][0] = 0; F_[1][1] = 1;         F_[1][2] = dt;
    F_[2][0] = 0; F_[2][1] = 0;         F_[2][2] = 1;

    // Measurement matrix H: we only measure position
    H_[0] = 1; H_[1] = 0; H_[2] = 0;

    // Process noise covariance Q (discretized white-noise-acceleration model)
    float dt2 = dt * dt, dt3 = dt2 * dt, dt4 = dt3 * dt;
    float q = processNoise;
    Q_[0][0] = dt4 / 4.0f * q; Q_[0][1] = dt3 / 2.0f * q; Q_[0][2] = dt2 / 2.0f * q;
    Q_[1][0] = dt3 / 2.0f * q; Q_[1][1] = dt2 * q;        Q_[1][2] = dt * q;
    Q_[2][0] = dt2 / 2.0f * q; Q_[2][1] = dt * q;         Q_[2][2] = q;

    // Measurement noise covariance R
    R_ = measNoiseStd * measNoiseStd;

    // Initial state and covariance (large uncertainty until we get data)
    x_[0] = 0; x_[1] = 0; x_[2] = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            P_[i][j] = (i == j) ? 1e3f : 0.0f;
        }
    }

    initialized_ = false;

}

// Feed one new measurement (position/angle at this timestep)
void Kalman::update(float measurement) {

    Kalman::lastValueMillis = millis();

    if (!initialized_) {
        x_[0] = measurement; x_[1] = 0; x_[2] = 0;
        initialized_ = true;
        return;
    }

    // ---- Predict ----
    float xPred[3];
    for (int i = 0; i < 3; i++) {
        xPred[i] = F_[i][0] * x_[0] + F_[i][1] * x_[1] + F_[i][2] * x_[2];
    }

    // PPred = F * P * F^T + Q
    float FP[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float s = 0;
            for (int k = 0; k < 3; k++) s += F_[i][k] * P_[k][j];
            FP[i][j] = s;
        }
    }

    float PPred[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float s = 0;
            for (int k = 0; k < 3; k++) s += FP[i][k] * F_[j][k]; // F^T
            PPred[i][j] = s + Q_[i][j];
        }
    }

    // ---- Update (measurement residual) ----
    float y = measurement - (H_[0] * xPred[0] + H_[1] * xPred[1] + H_[2] * xPred[2]);
    float S = PPred[0][0] + R_;  // scalar, since H picks out position only

    // Kalman gain K = PPred * H^T / S  (3x1), H^T = [1,0,0]
    float K[3] = { PPred[0][0] / S, PPred[1][0] / S, PPred[2][0] / S };

    // State update
    x_[0] = xPred[0] + K[0] * y;
    x_[1] = xPred[1] + K[1] * y;
    x_[2] = xPred[2] + K[2] * y;

    // Covariance update: P = (I - K*H) * PPred
    // Since H = [1,0,0], (I - K*H) only modifies column 0.
    float IKH[3][3];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            IKH[i][j] = (i == j ? 1.0f : 0.0f) - ((j == 0) ? K[i] : 0.0f);
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float s = 0;
            for (int k = 0; k < 3; k++) s += IKH[i][k] * PPred[k][j];
            P_[i][j] = s;
        }
    }

}

// Current filtered state
float Kalman::position() {
    return x_[0];
}

float Kalman::velocity() {
    return x_[1];
}

float Kalman::acceleration() {
    return x_[2];
}

// Predict position `stepsAhead * dt` seconds into the future
// using the current filtered state (no new measurements).
float Kalman::predict(float stepsAhead) {
    float t = stepsAhead * dt_;
    return x_[0] + x_[1] * t + 0.5f * x_[2] * t * t;
}
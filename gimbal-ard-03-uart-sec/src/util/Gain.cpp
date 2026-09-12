#include "Gain.h"

Gain::Gain(float g1, float g2, float m1, float p) {
    this->g1 = g1;
    this->g2 = g2;
    this->m1 = m1;
    this->p = p;
    this->recalculate();
}

// https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void Gain::recalculate() {
    this->o = pow(abs(this->g1 - this->g2), 1 / this->p) * sgn(this->g1 - this->g2);
    this->m2 = this->m1 / this->o;
}

float Gain::getGain(float value) {
    return pow((this->m1 - min(this->m1, abs(value))) / this->m2, this->p) + this->g2;
}

void Gain::setG1(float g1) {
    this->g1 = g1;
    this->recalculate();
}

void Gain::setG2(float g2) {
    this->g2 = g2;
    this->recalculate();
}

void Gain::setM1(float m1) {
    this->m1 = m1;
    this->recalculate();
}

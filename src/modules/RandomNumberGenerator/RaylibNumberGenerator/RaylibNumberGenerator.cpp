#include "RaylibNumberGenerator.hpp"

#include <climits>
#include <raylib.h>

void RaylibNumberGenerator::setSeed(uint32_t seed) {
    SetRandomSeed(seed);
}

float RaylibNumberGenerator::getRandomFloat() const {
    return float(GetRandomValue(0, INT_MAX)) / float(INT_MAX);
}

float RaylibNumberGenerator::getRandomFloat(float min, float max) const {
    return getRandomFloat() * (max - min) + min;
}

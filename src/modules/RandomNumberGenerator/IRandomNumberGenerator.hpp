#pragma once

#include <stdint.h>

class IRandomNumberGenerator {
public:
    virtual ~IRandomNumberGenerator() = default;

    virtual void setSeed(uint32_t seed) = 0;
    virtual float getRandomFloat() const = 0;
    virtual float getRandomFloat(float min, float max) const = 0;
};
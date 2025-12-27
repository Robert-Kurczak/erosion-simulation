#pragma once

#include "../IRandomNumberGenerator.hpp"

class RaylibNumberGenerator : public IRandomNumberGenerator{
public:
    virtual void setSeed(uint32_t seed) override;
    virtual float getRandomFloat() const override;
    virtual float getRandomFloat(float min, float max) const override;
};
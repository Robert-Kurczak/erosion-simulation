#pragma once

#include "IRainGenerator.hpp"
#include "RandomNumberGenerator/IRandomNumberGenerator.hpp"

class RainGenerator : public IRainGenerator {
private:
    IRandomNumberGenerator& randomNumberGenerator_;

public:
    RainGenerator(IRandomNumberGenerator& randomNumberGenerator);

    virtual void generateRainDrops(
        const BoundingBox& boundingBox,
        std::span<WaterDroplet> outputBuffer
    ) override;
};
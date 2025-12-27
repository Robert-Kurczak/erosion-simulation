#pragma once

#include "IRainGenerator.hpp"
#include "RandomNumberGenerator/IRandomNumberGenerator.hpp"

class RainGenerator : public IRainGenerator {
private:
    IRandomNumberGenerator& randomNumberGenerator_;

public:
    RainGenerator(IRandomNumberGenerator& randomNumberGenerator);

    virtual void generateRainDrops(
        uint32_t dropsAmount,
        uint32_t seed,
        const BoundingBox& boundingBox,
        std::span<RainDrop> outputBuffer
    ) override;
};
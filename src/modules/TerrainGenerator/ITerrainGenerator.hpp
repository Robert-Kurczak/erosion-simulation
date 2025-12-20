#pragma once

#include <span>
#include <stdint.h>

class ITerrainGenerator {
public:
    virtual void generateTerrain(
        uint32_t resolutionX,
        uint32_t resolutionZ,
        uint32_t seed,
        std::span<double> outputBuffer
    ) = 0;
};
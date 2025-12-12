#pragma once

#include <stdint.h>
#include <vector>

class ITerrainGenerator {
public:
    virtual std::vector<double> generateTerrain(
        uint32_t resolutionX,
        uint32_t resolutionZ,
        uint32_t seed
    ) = 0;
};
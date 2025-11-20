#pragma once

#include "Terrain/TerrainData.hpp"

#include <stdint.h>
#include <vector>

class ITerrainGenerator {
public:
    virtual TerrainData generateTerrain(
        uint32_t resolutionX,
        uint32_t resolutionZ
    ) = 0;
};
#pragma once

#include "TerrainData/TerrainData.hpp"

#include <raylib.h>
#include <stdint.h>

class ITerrainGenerator {
public:
    virtual Model generateTerrain(const TerrainData& terrainData) = 0;
};
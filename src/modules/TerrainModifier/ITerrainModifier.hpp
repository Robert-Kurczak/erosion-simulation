#pragma once

#include "Terrain/TerrainData.hpp"

class ITerrainModifier {
public:
    virtual void modify(TerrainData& terrainData) = 0;
};
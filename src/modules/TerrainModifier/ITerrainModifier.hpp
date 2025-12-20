#pragma once

#include "TerrainData/TerrainData.hpp"

class ITerrainModifier {
public:
    virtual void modify(TerrainData& terrainData) = 0;
};
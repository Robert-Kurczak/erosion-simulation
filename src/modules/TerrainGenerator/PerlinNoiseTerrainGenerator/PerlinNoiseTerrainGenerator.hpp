#pragma once

#include "../ITerrainGenerator.hpp"

class PerlinNoiseTerrainGenerator : public ITerrainGenerator {
public:
    virtual Model generateTerrain(const TerrainData& terrainData
    ) override;
};
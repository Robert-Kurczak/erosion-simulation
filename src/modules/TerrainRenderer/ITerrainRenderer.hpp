#pragma once

#include "Terrain/TerrainData.hpp"
#include "Terrain/TerrainModelConfig.hpp"

class ITerrainRenderer {
public:
    virtual void setupModel(
        const TerrainData& terrainData,
        const TerrainModelConfig& config
    ) = 0;

    virtual void renderModel(const TerrainData& terrainData) = 0;
};
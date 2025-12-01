#pragma once

#include "Terrain/TerrainData.hpp"
#include "Terrain/TerrainModelConfig.hpp"

#include <raylib.h>

class ITerrainRenderer {
public:
    virtual void setupModel(
        const TerrainData& terrainData,
        const TerrainModelConfig& config,
        const Vector3& lightSource
    ) = 0;

    virtual void renderModel(
        const TerrainData& terrainData,
        const Vector3& lightSource
    ) = 0;
};
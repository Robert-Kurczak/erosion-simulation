#pragma once

#include "TerrainData/TerrainData.hpp"

#include <raylib.h>

class ITerrainRenderer {
public:
    virtual ~ITerrainRenderer() = default;

    virtual void setupModel(
        const TerrainData& terrainData,
        const Vector3& lightSource
    ) = 0;

    virtual void renderModel(
        const TerrainData& terrainData,
        const Vector3& lightSource
    ) = 0;
};
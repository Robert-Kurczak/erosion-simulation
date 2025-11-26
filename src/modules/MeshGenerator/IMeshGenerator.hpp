#pragma once

#include "Terrain/TerrainData.hpp"

#include <raylib.h>

class IMeshGenerator {
public:
    virtual ~IMeshGenerator() = default;

    virtual Mesh generateMesh(
        const TerrainData& terrainData,
        const Vector3& worldSize
    ) = 0;

    virtual void updateMesh(
        Mesh& mesh,
        const TerrainData& terrainData
    ) = 0;
};
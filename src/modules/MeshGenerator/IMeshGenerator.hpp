#pragma once

#include "TerrainData/TerrainData.hpp"

#include <raylib.h>

class IMeshGenerator {
public:
    virtual ~IMeshGenerator() = default;

    virtual Mesh generateIlluminatedMesh(
        const TerrainData& terrainData,
        const Vector3& lightPosition
    ) = 0;

    virtual void updateIlluminatedMesh(
        Mesh& mesh,
        const TerrainData& terrainData,
        const Vector3& lightPosition
    ) = 0;
};
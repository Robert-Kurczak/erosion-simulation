#pragma once

#include "Terrain/TerrainData.hpp"

#include <raylib.h>

class IMeshGenerator {
public:
    virtual ~IMeshGenerator() = default;

    virtual Mesh generateMesh(const TerrainData& terrainData) = 0;
};
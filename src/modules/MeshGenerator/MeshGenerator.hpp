#pragma once

#include "IMeshGenerator.hpp"

#include <stdint.h>

class MeshGenerator : public IMeshGenerator {
public:
    virtual Mesh generateMesh(const TerrainData& terrainData) override;
};
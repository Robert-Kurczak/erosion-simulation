#pragma once

#include "IMeshGenerator.hpp"

#include <stdint.h>

class MeshGenerator : public IMeshGenerator {
private:
    Vector3 getPoint(
        const Vector2& index,
        const Vector3& scale,
        const Vector3& offset,
        const TerrainData& terrainData
    );

public:
    virtual Mesh generateMesh(
        const TerrainData& terrainData,
        const Vector3& worldSize
    ) override;

    virtual void updateMesh(Mesh& mesh, const TerrainData& terrainData)
        override;
};
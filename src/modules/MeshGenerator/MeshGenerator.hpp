#pragma once

#include "IMeshGenerator.hpp"

#include <stdint.h>

class MeshGenerator : public IMeshGenerator {
private:
    Vector3 getPoint(
        const Vector2& index,
        const TerrainData& terrainData,
        const Vector3& scale,
        const Vector3& offset
    );

    void setupVertices(
        Mesh& mesh,
        const TerrainData& data,
        const Vector3& worldSize
    );
    void setupTriangles(Mesh& mesh, const TerrainData& data);
    void setupTextureCoordinates(Mesh& mesh, const TerrainData& data);
    void setupLighting(Mesh& mesh, const TerrainData& data);

    void updateVertices(Mesh& mesh, const TerrainData& data);
    void updateLighting(Mesh& mesh, const TerrainData& data);

public:
    virtual Mesh generateMesh(
        const TerrainData& terrainData,
        const Vector3& worldSize
    ) override;

    virtual void updateMesh(
        Mesh& mesh,
        const TerrainData& terrainData,
        const Vector3& worldSize
    ) override;
};
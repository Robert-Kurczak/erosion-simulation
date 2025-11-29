#pragma once

#include "IMeshGenerator.hpp"

#include <stdint.h>

class MeshGenerator : public IMeshGenerator {
private:
    static constexpr uint8_t VERTICES_PER_QUAD = 6;
    static constexpr uint8_t COORDS_PER_VERTEX = 3;
    static constexpr uint8_t COORDS_PER_TEXTURE = 2;
    static constexpr uint8_t CHANNELS_PER_COLOR = 4;

    void setupMesh(
        Mesh& mesh,
        const TerrainData& data,
        uint32_t totalQuadsNumber
    );

    void addTriangles(
        Mesh& mesh,
        const TerrainData& data,
        const Vector3& worldSize,
        uint32_t startQuad,
        uint32_t endQuad
    );

    void addTextureCoords(
        Mesh& mesh,
        const TerrainData& data,
        uint32_t startQuad,
        uint32_t endQuad
    );

    void addLighting(
        Mesh& mesh,
        const TerrainData& data,
        uint32_t startQuad,
        uint32_t endQuad
    );

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
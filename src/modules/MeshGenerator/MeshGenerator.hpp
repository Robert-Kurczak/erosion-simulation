#pragma once

#include "IMeshGenerator.hpp"

#include <stdint.h>

class MeshGenerator : public IMeshGenerator {
private:
    static constexpr uint8_t VERTICES_PER_QUAD = 6;
    static constexpr uint8_t COORDS_PER_VERTEX = 3;
    static constexpr uint8_t COORDS_PER_TEXTURE = 2;
    static constexpr uint8_t CHANNELS_PER_COLOR = 4;

    Vector3 getWorldPoint(
        uint32_t xIndex,
        uint32_t zIndex,
        const TerrainData& data,
        const Vector3& offset,
        const Vector3& scale
    ) const;

    Vector2 getTextureCoord(
        uint32_t xIndex,
        uint32_t zIndex,
        const TerrainData& data
    ) const;

    Vector3 getVertexFromQuad(
        Mesh& mesh,
        uint32_t quadIndex,
        uint32_t vertexIndexInQuad
    );

    void addVertex(Mesh& mesh, uint32_t index, const Vector3& coords);
    void addTextureCoord(
        Mesh& mesh,
        uint32_t index,
        const Vector2& coords
    );
    void addColor(Mesh& mesh, uint32_t index, const Color& color);

    Color calculateLightIntensity(
        uint32_t xIndex,
        uint32_t zIndex,
        const TerrainData& data,
        const Vector3& worldSize,
        const Vector3& lightPosition,
        const Vector3& point
    );

    void setupMesh(Mesh& mesh, uint32_t totalQuadsNumber);

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
        const Vector3& worldSize,
        const Vector3& lightPosition,
        uint32_t startQuad,
        uint32_t endQuad
    );

    void updateTrianglesHeight(
        Mesh& mesh,
        const TerrainData& data,
        const Vector3& worldSize,
        uint32_t startQuad,
        uint32_t endQuad
    );

public:
    virtual Mesh generateIlluminatedMesh(
        const TerrainData& terrainData,
        const Vector3& worldSize,
        const Vector3& lightPosition
    ) override;

    virtual void updateIlluminatedMesh(
        Mesh& mesh,
        const TerrainData& terrainData,
        const Vector3& worldSize,
        const Vector3& lightPosition
    ) override;
};
#pragma once

#include "IMeshGenerator.hpp"

#include <stdint.h>
#include <thread>
#include <vector>

class MeshGenerator : public IMeshGenerator {
private:
    static constexpr uint8_t VERTICES_PER_QUAD_ = 6;
    static constexpr uint8_t COORDS_PER_VERTEX_ = 3;
    static constexpr uint8_t COORDS_PER_TEXTURE_ = 2;
    static constexpr uint8_t CHANNELS_PER_COLOR_ = 4;

    const uint32_t threads_amount_;
    std::vector<std::thread> generatorThreads_;
    std::vector<std::thread> updaterThreads_;

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
        const Vector3& lightPosition,
        const Vector3& point
    );

    void setupMesh(Mesh& mesh, uint32_t totalQuadsNumber);

    void addTriangles(
        Mesh& mesh,
        const TerrainData& data,
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
        const Vector3& lightPosition,
        uint32_t startQuad,
        uint32_t endQuad
    );

    void updateTrianglesHeight(
        Mesh& mesh,
        const TerrainData& data,
        uint32_t startQuad,
        uint32_t endQuad
    );

    std::thread createGeneratorThread(
        Mesh& mesh,
        const TerrainData& terrainData,
        const Vector3& lightPosition,
        uint32_t startQuad,
        uint32_t endQuad
    );

    std::thread createUpdaterThread(
        Mesh& mesh,
        const TerrainData& terrainData,
        const Vector3& lightPosition,
        uint32_t startQuad,
        uint32_t endQuad
    );

public:
    MeshGenerator();

    virtual Mesh generateIlluminatedMesh(
        const TerrainData& terrainData,
        const Vector3& lightPosition
    ) override;

    virtual void updateIlluminatedMesh(
        Mesh& mesh,
        const TerrainData& terrainData,
        const Vector3& lightPosition
    ) override;
};
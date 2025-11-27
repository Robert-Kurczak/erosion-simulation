#include "TerrainRenderer.hpp"

#include <cassert>
#include <limits>
#include <raylib.h>
#include <raymath.h>

Image TerrainRenderer::convertToTextureImage(
    const TerrainData& terrainData
) {
    assert(terrainData.resolutionX <= std::numeric_limits<int>::max());
    assert(terrainData.resolutionZ <= std::numeric_limits<int>::max());

    Image textureImage {
        .data = MemAlloc(
            terrainData.resolutionX * terrainData.resolutionZ * 4
        ),
        .width = static_cast<int>(terrainData.resolutionX),
        .height = static_cast<int>(terrainData.resolutionZ),
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
    };

    uint8_t* textureImageData = static_cast<uint8_t*>(textureImage.data);

    // TODO use std::memcpy?
    for (int y = 0; y < terrainData.resolutionZ; y++) {
        for (int x = 0; x < terrainData.resolutionX; x++) {
            const uint32_t pixelIndex = y * terrainData.resolutionX + x;
            const Color pixelValue = terrainData.colorMap[pixelIndex];

            textureImageData[pixelIndex * 4 + 0] = pixelValue.r;
            textureImageData[pixelIndex * 4 + 1] = pixelValue.g;
            textureImageData[pixelIndex * 4 + 2] = pixelValue.b;
            textureImageData[pixelIndex * 4 + 3] = pixelValue.a;
        }
    }

    return textureImage;
}

Vector3 TerrainRenderer::getCenteredPosition(const Model& model) {
    BoundingBox terrainBoundingBox =
        GetMeshBoundingBox(terrainModel_.meshes[0]);

    const Vector3 centerPosition = {
        -(terrainBoundingBox.min.x + terrainBoundingBox.max.x) / 2.0f,
        0,
        -(terrainBoundingBox.min.z + terrainBoundingBox.max.z) / 2.0f
    };

    return centerPosition;
}

TerrainRenderer::TerrainRenderer(IMeshGenerator& meshGenerator) :
    meshGenerator_(meshGenerator) {}

void TerrainRenderer::setupModel(
    const TerrainData& terrainData,
    const TerrainModelConfig& config
) {
    const Image textureImage = convertToTextureImage(terrainData);

    terrainMesh_ =
        meshGenerator_.generateMesh(terrainData, config.worldSize);

    UploadMesh(&terrainMesh_, true);

    terrainModel_ = LoadModelFromMesh(terrainMesh_);
    terrainTexture_ = LoadTextureFromImage(textureImage);

    terrainModel_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =
        terrainTexture_;

    terrainPosition_ = config.worldPosition;
    terrainWorldSize_ = config.worldSize;

    UnloadImage(textureImage);
}

void TerrainRenderer::renderModel(const TerrainData& terrainData) {
    meshGenerator_.updateMesh(
        terrainMesh_, terrainData, terrainWorldSize_
    );

    UpdateMeshBuffer(
        terrainMesh_,
        0,
        terrainMesh_.vertices,
        terrainMesh_.vertexCount,
        0
    );

    UpdateTexture(terrainTexture_, terrainData.colorMap.data());
    DrawModel(terrainModel_, terrainPosition_, 1.0f, WHITE);
}
#include "TerrainRenderer.hpp"

#include <cassert>
#include <limits>
#include <raylib.h>

Image TerrainRenderer::convertToHeightImage(const TerrainData& terrainData
) {
    assert(terrainData.resolutionX <= std::numeric_limits<int>::max());
    assert(terrainData.resolutionZ <= std::numeric_limits<int>::max());

    Image heightImage {
        .data =
            MemAlloc(terrainData.resolutionX * terrainData.resolutionZ),
        .width = static_cast<int>(terrainData.resolutionX),
        .height = static_cast<int>(terrainData.resolutionZ),
        .mipmaps = 1,
        .format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE,
    };

    uint8_t* heightImageData = static_cast<uint8_t*>(heightImage.data);

    for (int y = 0; y < terrainData.resolutionZ; y++) {
        for (int x = 0; x < terrainData.resolutionX; x++) {
            const uint32_t pixelIndex = y * terrainData.resolutionX + x;

            const uint8_t pixelValue =
                terrainData.heightMap[y * terrainData.resolutionX + x] *
                255.0;

            heightImageData[pixelIndex] = pixelValue;
        }
    }

    return heightImage;
}

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

void TerrainRenderer::setupModel(
    const TerrainData& terrainData,
    const TerrainModelConfig& config
) {
    const Image heightImage = convertToHeightImage(terrainData);
    const Image textureImage = convertToTextureImage(terrainData);
    const Mesh terrainMesh = GenMeshHeightmap(
        heightImage,
        Vector3 {config.worldSizeX, config.worldSizeY, config.worldSizeZ}
    );

    terrainModel_ = LoadModelFromMesh(terrainMesh);
    terrainTexture_ = LoadTextureFromImage(textureImage);

    terrainModel_.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture =
        terrainTexture_;

    terrainPosition_ = getCenteredPosition(terrainModel_);
    terrainPosition_.x += config.worldPositionX;
    terrainPosition_.y += config.worldPositionY;
    terrainPosition_.z += config.worldPositionZ;

    UnloadImage(heightImage);
    UnloadImage(textureImage);
}

void TerrainRenderer::renderModel(const TerrainData& terrainData) {
    DrawModel(terrainModel_, terrainPosition_, 1.0f, WHITE);
    UpdateTexture(terrainTexture_, terrainData.colorMap.data());
}
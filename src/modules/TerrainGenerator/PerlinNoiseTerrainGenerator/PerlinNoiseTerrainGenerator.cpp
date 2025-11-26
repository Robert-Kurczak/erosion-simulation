#include "PerlinNoiseTerrainGenerator.hpp"

#include <raylib.h>

std::vector<double> PerlinNoiseTerrainGenerator::
    createNormalizedHeightMap(const Image& heightMapImage) {
    const uint8_t channelsPerPixel = 4;

    const size_t pixelsInImage =
        heightMapImage.width * heightMapImage.height;

    const uint8_t* pixels = static_cast<uint8_t*>(heightMapImage.data);

    std::vector<double> heightMap(pixelsInImage);

    for (size_t pixelIndex = 0; pixelIndex < pixelsInImage;
         pixelIndex++) {
        const double height =
            pixels[pixelIndex * channelsPerPixel] / 255.0f;

        heightMap[pixelIndex] = height;
    }

    return heightMap;
}

std::vector<Color> PerlinNoiseTerrainGenerator::createInitialColorMap(
    const Image& heightMapImage
) {
    const size_t pixelsInImage =
        heightMapImage.width * heightMapImage.height;

    const std::vector<Color> colorMap(pixelsInImage, RED);

    return colorMap;
}

TerrainData PerlinNoiseTerrainGenerator::generateTerrain(
    uint32_t resolutionX,
    uint32_t resolutionZ,
    float featureSize,
    uint32_t seed
) {
    const Image perlinImage = GenImagePerlinNoise(
        resolutionX,
        resolutionZ,
        resolutionX * (seed % 256),
        resolutionZ * (seed % 512),
        featureSize
    );

    TerrainData terrainData {
        .resolutionX = resolutionX,
        .resolutionZ = resolutionZ,
        .heightMap = createNormalizedHeightMap(perlinImage),
        .colorMap = createInitialColorMap(perlinImage)
    };

    return terrainData;
}
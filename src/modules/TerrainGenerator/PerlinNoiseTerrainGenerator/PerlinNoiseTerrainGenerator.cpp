#include "PerlinNoiseTerrainGenerator.hpp"

std::vector<double> PerlinNoiseTerrainGenerator::
    createNormalizedHeightMap(
        const FastNoiseLite& noiseGenerator,
        uint32_t resolutionX,
        uint32_t resolutionZ
    ) {
    std::vector<double> heightMap(resolutionX * resolutionZ);

    uint32_t index = 0;
    for (uint32_t y = 0; y < resolutionZ; y++) {
        for (uint32_t x = 0; x < resolutionX; x++) {
            heightMap[index] =
                (noiseGenerator.GetNoise(float(x), float(y)) + 1.0f) /
                2.0f;

            index++;
        }
    }

    return heightMap;
}

std::vector<Color> PerlinNoiseTerrainGenerator::createInitialColorMap(
    uint32_t resolutionX,
    uint32_t resolutionZ
) {
    const std::vector<Color> colorMap(resolutionX * resolutionZ, RED);
    return colorMap;
}

TerrainData PerlinNoiseTerrainGenerator::generateTerrain(
    uint32_t resolutionX,
    uint32_t resolutionZ,
    float featureSize,
    uint32_t seed
) {
    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseGenerator.SetFractalType(FastNoiseLite::FractalType_FBm);
    noiseGenerator.SetSeed(seed);
    noiseGenerator.SetFractalLacunarity(1.85f);
    noiseGenerator.SetFractalOctaves(6);
    noiseGenerator.SetFractalGain(0.4f);
    noiseGenerator.SetFrequency(0.8f / float(resolutionX));

    const TerrainData terrainData {
        .resolutionX = resolutionX,
        .resolutionZ = resolutionZ,
        .heightMap = createNormalizedHeightMap(
            noiseGenerator, resolutionX, resolutionZ
        ),
        .colorMap = createInitialColorMap(resolutionX, resolutionZ)
    };

    return terrainData;
}
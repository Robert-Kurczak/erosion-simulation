#include "PerlinNoiseTerrainGenerator.hpp"

void PerlinNoiseTerrainGenerator::fillHeightMap(
    const FastNoiseLite& noiseGenerator,
    uint32_t resolutionX,
    uint32_t resolutionZ,
    std::span<double> outputBuffer
) {
    uint32_t index = 0;
    for (uint32_t y = 0; y < resolutionZ; y++) {
        for (uint32_t x = 0; x < resolutionX; x++) {
            outputBuffer[index] =
                (noiseGenerator.GetNoise(float(x), float(y)) + 1.0f) /
                2.0f;

            index++;
        }
    }
}

void PerlinNoiseTerrainGenerator::generateTerrain(
    uint32_t resolutionX,
    uint32_t resolutionZ,
    uint32_t seed,
    std::span<double> outputBuffer
) {
    FastNoiseLite noiseGenerator;
    noiseGenerator.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noiseGenerator.SetFractalType(FastNoiseLite::FractalType_FBm);
    noiseGenerator.SetSeed(seed);
    noiseGenerator.SetFractalLacunarity(1.85f);
    noiseGenerator.SetFractalOctaves(6);
    noiseGenerator.SetFractalGain(0.4f);
    noiseGenerator.SetFrequency(0.8f / float(resolutionX));

    fillHeightMap(noiseGenerator, resolutionX, resolutionZ, outputBuffer);
}
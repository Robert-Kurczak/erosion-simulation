#pragma once

#include "../ITerrainGenerator.hpp"

#include <FastNoiseLite.h>
#include <vector>

class PerlinNoiseTerrainGenerator : public ITerrainGenerator {
private:
    void fillHeightMap(
        const FastNoiseLite& noiseGenerator,
        uint32_t resolutionX,
        uint32_t resolutionZ,
        std::span<double> outputBuffer
    );

public:
    virtual void generateTerrain(
        uint32_t resolutionX,
        uint32_t resolutionZ,
        uint32_t seed,
        std::span<double> outputBuffer
    ) override;
};
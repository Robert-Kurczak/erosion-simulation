#pragma once

#include "../ITerrainGenerator.hpp"

#include <FastNoiseLite.h>

class PerlinNoiseTerrainGenerator : public ITerrainGenerator {
private:
    std::vector<double> createNormalizedHeightMap(
        const FastNoiseLite& noiseGenerator,
        uint32_t resolutionX,
        uint32_t resolutionZ
    );

public:
    virtual std::vector<double> generateTerrain(
        uint32_t resolutionX,
        uint32_t resolutionZ,
        uint32_t seed
    ) override;
};
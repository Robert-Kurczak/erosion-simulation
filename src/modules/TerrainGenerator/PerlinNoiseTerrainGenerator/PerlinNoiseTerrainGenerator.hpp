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

    std::vector<Color> createInitialColorMap(
        uint32_t resolutionX,
        uint32_t resolutionZ
    );

public:
    virtual TerrainData generateTerrain(
        uint32_t resolutionX,
        uint32_t resolutionY,
        float featureSize,
        uint32_t seed
    ) override;
};
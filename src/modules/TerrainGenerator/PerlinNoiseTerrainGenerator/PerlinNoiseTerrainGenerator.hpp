#pragma once

#include "../ITerrainGenerator.hpp"

class PerlinNoiseTerrainGenerator : public ITerrainGenerator {
private:
    std::vector<double> createNormalizedHeightMap(
        const Image& heightMapImage
    );

    std::vector<Color> createInitialColorMap(const Image& heightMapImage);

public:
    virtual TerrainData generateTerrain(
        uint32_t resolutionX,
        uint32_t resolutionY,
        float featureSize,
        uint32_t seed
    ) override;
};
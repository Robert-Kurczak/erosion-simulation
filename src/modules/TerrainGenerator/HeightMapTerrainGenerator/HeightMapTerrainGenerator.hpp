#pragma once

#include "../ITerrainGenerator.hpp"

class HeightMapTerrainGenerator : public ITerrainGenerator {
public:
    virtual Model generateTerrain() override;
};
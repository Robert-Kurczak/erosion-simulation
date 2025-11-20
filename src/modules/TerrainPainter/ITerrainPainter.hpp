#pragma once

#include "Terrain/TerrainData.hpp"

#include <raylib.h>

class ITerrainPainter {
public:
    virtual void paint(
        const std::vector<double>& heightMap,
        std::vector<Color>& colorMap
    ) = 0;
};
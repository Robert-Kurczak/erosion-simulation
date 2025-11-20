#pragma once

#include "../ITerrainPainter.hpp"

class SimpleTerrainPainter : public ITerrainPainter {
public:
    virtual void paint(
        const std::vector<double>& heightMap,
        std::vector<Color>& colorMap
    ) override;
};
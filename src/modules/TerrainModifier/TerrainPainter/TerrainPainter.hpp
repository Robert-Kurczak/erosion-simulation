#pragma once

#include "../ITerrainModifier.hpp"

class TerrainPainter : public ITerrainModifier {
public:
    virtual void modify(TerrainData& terrainData) override;
};
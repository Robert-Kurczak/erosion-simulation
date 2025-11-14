#pragma once

#include "raylib.h"

class ITerrainGenerator {
public:
    virtual Model generateTerrain() = 0;
};
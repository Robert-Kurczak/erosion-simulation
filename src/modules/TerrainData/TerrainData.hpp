#pragma once

#include <stdint.h>
#include <vector>

struct TerrainData {
    uint32_t width;
    uint32_t length;
    std::vector<double> heightMap;

    double& at(uint32_t x, uint32_t y) {
        return heightMap[y * width + x];
    }
};
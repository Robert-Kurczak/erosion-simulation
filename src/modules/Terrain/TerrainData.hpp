#pragma once

#include <raylib.h>
#include <stdint.h>
#include <vector>

struct TerrainData {
    uint32_t resolutionX;
    uint32_t resolutionZ;

    std::vector<double> heightMap;
    std::vector<Color> colorMap;

    double& heightAt(uint32_t x, uint32_t y) {
        return heightMap[y * resolutionX + x];
    }

    Color& colorAt(uint32_t x, uint32_t y) {
        return colorMap[y * resolutionX + x];
    }
};
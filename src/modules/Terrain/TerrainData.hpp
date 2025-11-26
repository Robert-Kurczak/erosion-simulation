#pragma once

#include <raylib.h>
#include <stdint.h>
#include <vector>

struct TerrainData {
    uint32_t resolutionX;
    uint32_t resolutionZ;

    std::vector<double> heightMap;
    std::vector<Color> colorMap;

    double heightAt(const Vector2& index) const {
        return heightMap[index.y * resolutionX + index.x];
    }

    Color colorAt(const Vector2& index) const {
        return colorMap[index.y * resolutionX + index.x];
    }
};
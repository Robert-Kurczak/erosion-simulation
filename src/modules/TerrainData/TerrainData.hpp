#pragma once

#include "RainGenerator/RainDrop.hpp"

#include <raylib.h>
#include <stdint.h>
#include <vector>

struct TerrainData {
    uint32_t resolutionX;
    uint32_t resolutionZ;
    Vector3 worldSize;
    Vector3 worldPosition;

    std::vector<double> heightMap;
    std::vector<Color> colorMap;
    std::vector<RainDrop> rainMap;

    double heightAt(uint32_t x, uint32_t z) const {
        return heightMap[z * resolutionX + x];
    }

    double& mutableHeightAtWorldPosition(const Vector2& position) {
        const uint32_t x = (position.x / worldSize.x) * resolutionX;
        const uint32_t z = (position.y / worldSize.z) * resolutionZ;
        return heightMap[z * resolutionX + x];
    }
};
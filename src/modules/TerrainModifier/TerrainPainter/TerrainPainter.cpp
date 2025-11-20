#include "TerrainPainter.hpp"

Color TerrainPainter::convertHeightToColor(double height) {
    if (height < 0) {
        height = 0;
    } else if (height > 1) {
        height = 1;
    }

    const uint8_t colorIndex = COLOR_GRADIENT_.size() * height;
    return COLOR_GRADIENT_[colorIndex];
}

void TerrainPainter::modify(TerrainData& terrainData) {
    for (size_t i = 0; i < terrainData.heightMap.size(); i++) {
        const double height = terrainData.heightMap[i];
        terrainData.colorMap[i] = convertHeightToColor(height);
    }
}
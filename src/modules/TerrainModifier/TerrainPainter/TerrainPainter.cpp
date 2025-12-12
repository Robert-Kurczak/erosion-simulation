#include "TerrainPainter.hpp"

Color TerrainPainter::convertHeightToColor(double height) {
    if (height < 0) {
        height = 0;
    } else if (height > 1) {
        height = 1;
    }

    const double scaledColorIndex = (COLOR_GRADIENT_.size() - 1) * height;
    const uint8_t lowerColorIndex = uint8_t(scaledColorIndex);
    const uint8_t upperColorIndex = uint8_t(scaledColorIndex + 1);
    const double colorSlope = scaledColorIndex - lowerColorIndex;

    const Color lowerColor = COLOR_GRADIENT_[lowerColorIndex];
    const Color upperColor = COLOR_GRADIENT_[upperColorIndex];

    const Color interpolatedColor {
        uint8_t(
            lowerColor.r + (upperColor.r - lowerColor.r) * colorSlope
        ),
        uint8_t(
            lowerColor.g + (upperColor.g - lowerColor.g) * colorSlope
        ),
        uint8_t(
            lowerColor.b + (upperColor.b - lowerColor.b) * colorSlope
        ),
        255
    };

    return interpolatedColor;
}

void TerrainPainter::modify(TerrainData& terrainData) {
    for (size_t i = 0; i < terrainData.heightMap.size(); i++) {
        const double height = terrainData.heightMap[i];
        terrainData.colorMap[i] = convertHeightToColor(height);
    }
}
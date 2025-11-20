#include "TerrainPainter.hpp"

void TerrainPainter::modify(TerrainData& terrainData) {
    for (size_t i = 0; i < terrainData.heightMap.size(); i++) {
        terrainData.colorMap[i] = BLUE;
    }
}
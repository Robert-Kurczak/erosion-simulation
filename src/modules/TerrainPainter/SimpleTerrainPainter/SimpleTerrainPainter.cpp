#include "SimpleTerrainPainter.hpp"

void SimpleTerrainPainter::paint(
    const std::vector<double>& heightMap,
    std::vector<Color>& colorMap
) {
    for (size_t i = 0; i < heightMap.size(); i++) {
        colorMap[i] = BLUE;
    }
}
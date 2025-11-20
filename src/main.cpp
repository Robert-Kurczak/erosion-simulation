#include "Application/Application.hpp"
#include "Application/ApplicationConfig.hpp"
#include "Scene/TerrainScene/TerrainScene.hpp"
#include "TerrainGenerator/PerlinNoiseTerrainGenerator/PerlinNoiseTerrainGenerator.hpp"
#include "TerrainPainter/SimpleTerrainPainter/SimpleTerrainPainter.hpp"
#include "TerrainRenderer/TerrainRenderer.hpp"

#include <raylib.h>

ApplicationConfig applicationConfig_ {
    .name = "Erosion Simulator",
    .windowWidthScale = 0.8,
    .windowHeightScale = 0.8,
    .framesPerSeconds = 120
};

PerlinNoiseTerrainGenerator terrainGenerator_ {};

SimpleTerrainPainter terrainPainter_ {};

TerrainRenderer terrainRenderer_ {};

TerrainScene terrainScene_ {
    terrainGenerator_,
    terrainPainter_,
    terrainRenderer_
};

Application application_ {applicationConfig_, terrainScene_};

int main(void) {
    application_.setup();
    application_.enterMainLoop();

    return 0;
}
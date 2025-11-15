#include "Application/Application.hpp"
#include "Application/ApplicationConfig.hpp"
#include "Scene/TerrainScene/TerrainScene.hpp"
#include "TerrainGenerator/PerlinNoiseTerrainGenerator/PerlinNoiseTerrainGenerator.hpp"

#include <raylib.h>

ApplicationConfig applicationConfig_ {
    .name = "Erosion Simulator",
    .windowWidthScale = 0.8,
    .windowHeightScale = 0.8,
    .framesPerSeconds = 120
};

PerlinNoiseTerrainGenerator terrainGenerator_ {};

TerrainScene terrainScene_ {terrainGenerator_};

Application application_ {applicationConfig_, terrainScene_};

int main(void) {
    application_.setup();
    application_.enterMainLoop();

    return 0;
}
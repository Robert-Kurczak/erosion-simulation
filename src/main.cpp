#include "Application/Application.hpp"
#include "Application/ApplicationConfig.hpp"
#include "MeshGenerator/MeshGenerator.hpp"
#include "Scene/TerrainScene/TerrainScene.hpp"
#include "TerrainGenerator/PerlinNoiseTerrainGenerator/PerlinNoiseTerrainGenerator.hpp"
#include "TerrainModifier/TerrainPainter/TerrainPainter.hpp"
#include "TerrainRenderer/TerrainRenderer.hpp"

#include <array>
#include <raylib.h>

ApplicationConfig applicationConfig_ {
    .name = "Erosion Simulator",
    .windowWidthScale = 0.8,
    .windowHeightScale = 0.8,
    .framesPerSeconds = 120
};

PerlinNoiseTerrainGenerator terrainGenerator_ {};

TerrainPainter terrainPainter_ {};

const std::vector<ITerrainModifier*> terrainModifiers_ {&terrainPainter_};

MeshGenerator meshGenerator_ {};

TerrainRenderer terrainRenderer_ {meshGenerator_};

TerrainScene terrainScene_ {
    terrainGenerator_,
    terrainModifiers_,
    terrainRenderer_
};

Application application_ {applicationConfig_, terrainScene_};

int main(void) {
    application_.setup();
    application_.enterMainLoop();

    return 0;
}
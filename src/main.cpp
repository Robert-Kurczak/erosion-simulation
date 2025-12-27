#include "Application/Application.hpp"
#include "Application/ApplicationConfig.hpp"
#include "InputController/KeyboardInputController.hpp"
#include "MeshGenerator/MeshGenerator.hpp"
#include "RainGenerator/RainGenerator.hpp"
#include "RandomNumberGenerator/RaylibNumberGenerator/RaylibNumberGenerator.hpp"
#include "Scene/TerrainScene/TerrainScene.hpp"
#include "TerrainGenerator/PerlinNoiseTerrainGenerator/PerlinNoiseTerrainGenerator.hpp"
#include "TerrainModifier/HydraulicErosion/HydraulicErosion.hpp"
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

KeyboardInputController keyboardInputController_ {};

PerlinNoiseTerrainGenerator terrainGenerator_ {};

RaylibNumberGenerator randomNumberGenerator_ {};

RainGenerator rainGenerator_ {randomNumberGenerator_};

TerrainPainter terrainPainter_ {};

HydraulicErosion hydraulicErosion_ {randomNumberGenerator_};

const std::vector<ITerrainModifier*> terrainModifiers_ {
    &terrainPainter_,
    &hydraulicErosion_
};

MeshGenerator meshGenerator_ {};

TerrainRenderer terrainRenderer_ {meshGenerator_};

TerrainScene terrainScene_ {
    keyboardInputController_,
    terrainGenerator_,
    rainGenerator_,
    terrainModifiers_,
    terrainRenderer_
};

Application application_ {applicationConfig_, terrainScene_};

int main(void) {
    application_.setup();
    application_.enterMainLoop();

    return 0;
}
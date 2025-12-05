#include "TerrainScene.hpp"

#include "Terrain/TerrainModelConfig.hpp"

#include <cstring>

void TerrainScene::setupCamera() {
    mainCamera_.position = Vector3 {-50.0f, 120.0f, -150.0f};
    mainCamera_.target = Vector3 {0.0f, 40.0f, 0.0f};
    mainCamera_.up = Vector3 {0.0f, 1.0f, 0.0f};
    mainCamera_.fovy = 45.0f;
    mainCamera_.projection = CAMERA_PERSPECTIVE;
}

void TerrainScene::setupTerrain() {
    const uint32_t resolutionX = 1024;
    const uint32_t resolutionZ = 1024;
    const uint32_t seed = 20;

    terrainData_ =
        terrainGenerator_.generateTerrain(resolutionX, resolutionZ, seed);

    TerrainModelConfig terrainModelConfig {
        .worldSize =
            Vector3 {
                     .x = 256,
                     .y = 64,
                     .z = 256,
                     },
        .worldPosition = Vector3 {.x = 0, .y = 0, .z = 0}
    };

    terrainRenderer_.setupModel(
        terrainData_, terrainModelConfig, lightSourcePosition_
    );
}

void TerrainScene::drawUi() {
    const Color fontColor = WHITE;
    const uint8_t fontSize = 30;
    const uint8_t leftMargin = 10;
    const uint8_t topMargin = 10;

    DrawText(
        TextFormat("FPS: %d", GetFPS()),
        leftMargin,
        topMargin,
        fontSize,
        fontColor
    );

    DrawText(
        TextFormat("Vertices: %d", terrainData_.heightMap.size()),
        leftMargin,
        fontSize + topMargin,
        fontSize,
        fontColor
    );
}

TerrainScene::TerrainScene(
    ITerrainGenerator& terrainGenerator,
    const std::vector<ITerrainModifier*>& terrainModifiers,
    ITerrainRenderer& terrainRenderer
) :
    terrainGenerator_(terrainGenerator),
    terrainModifiers_(terrainModifiers),
    terrainRenderer_(terrainRenderer) {}

void TerrainScene::setup() {
    setupCamera();
    setupTerrain();
}

void TerrainScene::draw() {
    BeginDrawing();
    BeginMode3D(mainCamera_);

    ClearBackground(BLACK);

    for (const auto& modifier : terrainModifiers_) {
        modifier->modify(terrainData_);
    }

    terrainRenderer_.renderModel(terrainData_, lightSourcePosition_);

    EndMode3D();
    drawUi();
    EndDrawing();
}
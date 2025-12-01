#include "TerrainScene.hpp"

#include "Terrain/TerrainModelConfig.hpp"

#include <cstring>

void TerrainScene::setupCamera() {
    mainCamera_.position = Vector3 {30.0f, 100.0f, 100.0f};
    mainCamera_.target = Vector3 {0.0f, 0.0f, 0.0f};
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
                     .x = 128,
                     .y = 64,
                     .z = 128,
                     },
        .worldPosition = Vector3 {.x = 0, .y = 0, .z = 0}
    };

    terrainRenderer_.setupModel(
        terrainData_, terrainModelConfig, lightSourcePosition_
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

    ClearBackground(DARKGRAY);

    for (const auto& modifier : terrainModifiers_) {
        modifier->modify(terrainData_);
    }

    terrainRenderer_.renderModel(terrainData_, lightSourcePosition_);

    EndMode3D();
    DrawText(TextFormat("FPS: %d", GetFPS()), 10, 10, 30, BLACK);

    EndDrawing();
}
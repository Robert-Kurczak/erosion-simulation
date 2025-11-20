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
    terrainData_ = terrainGenerator_.generateTerrain(512, 512);

    TerrainModelConfig terrainModelConfig {
        .worldSizeX = 128,
        .worldSizeY = 16,
        .worldSizeZ = 128,
        .worldPositionX = 0,
        .worldPositionY = 0,
        .worldPositionZ = 0
    };

    terrainRenderer_.setupModel(terrainData_, terrainModelConfig);
}

TerrainScene::TerrainScene(
    ITerrainGenerator& terrainGenerator,
    ITerrainPainter& terrainPainter,
    ITerrainRenderer& terrainRenderer
) :
    terrainGenerator_(terrainGenerator),
    terrainPainter_(terrainPainter),
    terrainRenderer_(terrainRenderer) {}

void TerrainScene::setup() {
    setupCamera();
    setupTerrain();
}

void TerrainScene::draw() {
    BeginDrawing();
    BeginMode3D(mainCamera_);

    ClearBackground(RAYWHITE);
    terrainRenderer_.renderModel(terrainData_);

    EndMode3D();
    EndDrawing();
}
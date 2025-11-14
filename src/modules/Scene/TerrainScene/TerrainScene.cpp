#include "TerrainScene.hpp"

void TerrainScene::setupCamera() {
    mainCamera_.position = Vector3 {18.0f, 21.0f, 18.0f};
    mainCamera_.target = Vector3 {0.0f, 0.0f, 0.0f};
    mainCamera_.up = Vector3 {0.0f, 1.0f, 0.0f};
    mainCamera_.fovy = 45.0f;
    mainCamera_.projection = CAMERA_PERSPECTIVE;
}

void TerrainScene::setupTerrain() {
    terrainModel_ = terrainGenerator_.generateTerrain();
    terrainModelPosition_ = {-8.0f, 0.0f, -8.0f};
}

TerrainScene::TerrainScene(ITerrainGenerator& terrainGenerator) :
    terrainGenerator_(terrainGenerator) {}

void TerrainScene::setup() {
    setupCamera();
    setupTerrain();
}

void TerrainScene::draw() {
    BeginDrawing();
    BeginMode3D(mainCamera_);

    ClearBackground(RAYWHITE);
    DrawModel(terrainModel_, terrainModelPosition_, 1.0f, RED);

    EndMode3D();
    EndDrawing();
}
#include "TerrainScene.hpp"

void TerrainScene::setupCamera() {
    mainCamera_.position = Vector3 {30.0f, 100.0f, 100.0f};
    mainCamera_.target = Vector3 {0.0f, 0.0f, 0.0f};
    mainCamera_.up = Vector3 {0.0f, 1.0f, 0.0f};
    mainCamera_.fovy = 45.0f;
    mainCamera_.projection = CAMERA_PERSPECTIVE;
}

void TerrainScene::setupTerrain() {
    terrainModel_ = terrainGenerator_.generateTerrain();

    BoundingBox terrainBoundingBox =
        GetMeshBoundingBox(terrainModel_.meshes[0]);

    Vector3 terrainModelCenter = {
        (terrainBoundingBox.min.x + terrainBoundingBox.max.x) / 2.0f,
        (terrainBoundingBox.min.y + terrainBoundingBox.max.y) / 2.0f,
        (terrainBoundingBox.min.z + terrainBoundingBox.max.z) / 2.0f
    };

    terrainModelPosition_ = {
        -terrainModelCenter.x, 0.0f, -terrainModelCenter.z
    };
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
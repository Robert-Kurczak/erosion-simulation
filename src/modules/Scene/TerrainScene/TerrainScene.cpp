#include "TerrainScene.hpp"

#include <cstring>
#include <raymath.h>

void TerrainScene::setupTerrain() {
    const uint32_t resolutionX = 1024;
    const uint32_t resolutionZ = 1024;

    terrainData_.resolutionX = resolutionX;
    terrainData_.resolutionZ = resolutionZ;

    terrainData_.heightMap = terrainGenerator_.generateTerrain(
        resolutionX, resolutionZ, terrainSeed_
    );

    terrainData_.colorMap = std::vector<Color>(resolutionX * resolutionZ);

    terrainData_.rainMap = rainGenerator_.generateRainDrops(
        terrainModelConfig_.worldSize, rainDropsAmount_, terrainSeed_
    );

    terrainRenderer_.setupModel(
        terrainData_, terrainModelConfig_, lightSourcePosition_
    );
}

void TerrainScene::setupCamera() {
    mainCamera_.position = Vector3 {-50.0f, 120.0f, -150.0f};
    mainCamera_.target = Vector3 {0.0f, 40.0f, 0.0f};
    mainCamera_.up = Vector3 {0.0f, 1.0f, 0.0f};
    mainCamera_.fovy = 45.0f;
    mainCamera_.projection = CAMERA_PERSPECTIVE;
}

void TerrainScene::setCameraZoom(float zoom) {
    const Vector3 cameraDirection = Vector3Normalize(
        Vector3Subtract(mainCamera_.position, mainCamera_.target)
    );

    const Vector3 zoomVector =
        Vector3Scale(cameraDirection, zoom * zoomSpeed_);

    mainCamera_.position = Vector3Add(mainCamera_.position, zoomVector);
}

void TerrainScene::renderModels() {
    ClearBackground(BLACK);

    for (const auto& modifier : terrainModifiers_) {
        modifier->modify(terrainData_);
    }

    terrainRenderer_.renderModel(terrainData_, lightSourcePosition_);
}

void TerrainScene::renderUi() {
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

void TerrainScene::handleInput() {
    const bool shouldRegenerateTerrain =
        inputController_.isActionPressed(InputAction::RegenerateTerrain);

    const bool shouldLookAround =
        inputController_.isActionDown(InputAction::LookAround);

    if (shouldRegenerateTerrain) {
        terrainSeed_++;
        setupTerrain();
    }

    if (shouldLookAround) {
        UpdateCamera(&mainCamera_, CAMERA_THIRD_PERSON);
    }

    const float zoom =
        inputController_.getActionValue(InputAction::CameraZoom);

    if (zoom != 0) {
        setCameraZoom(zoom);
    }
}

TerrainScene::TerrainScene(
    IInputController& inputController,
    ITerrainGenerator& terrainGenerator,
    IRainGenerator& rainGenerator,
    const std::vector<ITerrainModifier*>& terrainModifiers,
    ITerrainRenderer& terrainRenderer
) :
    inputController_(inputController),
    terrainGenerator_(terrainGenerator),
    rainGenerator_(rainGenerator),
    terrainModifiers_(terrainModifiers),
    terrainRenderer_(terrainRenderer) {}

void TerrainScene::setup() {
    setupTerrain();
    setupCamera();
}

void TerrainScene::draw() {
    BeginDrawing();

    BeginMode3D(mainCamera_);
    renderModels();
    handleInput();
    EndMode3D();

    renderUi();
    EndDrawing();
}
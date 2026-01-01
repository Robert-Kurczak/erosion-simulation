#include "TerrainScene.hpp"

#include <cstring>
#include <raymath.h>

void TerrainScene::setupTerrain() {
    terrainGenerator_.generateTerrain(
        resolutionX_,
        resolutionZ_,
        terrainSeed_,
        terrainData_.getHeightMap()
    );

    rainGenerator_.generateRainDrops(
        terrainData_.getBoundingBox(), terrainData_.getRainMap()
    );

    terrainRenderer_.setupModel(terrainData_, lightSourcePosition_);

    for (const auto& modifier : terrainModifiers_) {
        modifier->modify(terrainData_);
    }
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
        Vector3Scale(cameraDirection, -zoom * zoomSpeed_);

    mainCamera_.position = Vector3Add(mainCamera_.position, zoomVector);
}

void TerrainScene::renderModels() {
    ClearBackground(BLACK);
    terrainRenderer_.renderModel(terrainData_, lightSourcePosition_);

    if (!simulationStarted_) {
        return;
    }

    for (const auto& modifier : terrainModifiers_) {
        modifier->modify(terrainData_);
    }
}

void TerrainScene::drawKeybinds(
    uint8_t fontSize,
    const Color& fontColor,
    const Vector2& margins
) {
    uint32_t counter = 0;
    for (const auto& actionBinding : inputController_.getBindings()) {
        const InputAction& action = actionBinding.first;
        const InputBinding& binding = actionBinding.second;

        const std::string_view actionName =
            inputController_.getActionName(action);
        const std::string_view bindingName =
            inputController_.getBindingName(binding);

        DrawText(
            TextFormat("%s - %s", actionName.data(), bindingName.data()),
            margins.x,
            fontSize * counter + margins.y,
            fontSize,
            fontColor
        );

        counter++;
    }
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
        TextFormat("Vertices: %d", terrainData_.getHeightMap().size()),
        leftMargin,
        fontSize + topMargin,
        fontSize,
        fontColor
    );

    DrawText(
        TextFormat("Rain drops: %d", terrainData_.getRainMap().size()),
        leftMargin,
        2 * fontSize + topMargin,
        fontSize,
        fontColor
    );

    drawKeybinds(fontSize, WHITE, {10.0f, 5 * fontSize + topMargin});
}

void TerrainScene::handleInput() {
    if (inputController_.isActionReleased(InputAction::StartSimulation)) {
        simulationStarted_ = !simulationStarted_;
    }

    const bool shouldRegenerateTerrain =
        inputController_.isActionPressed(InputAction::RegenerateTerrain);

    const bool shouldResetScene =
        inputController_.isActionPressed(InputAction::ResetScene);

    const bool shouldLookAround =
        inputController_.isActionDown(InputAction::LookAround);

    if (shouldRegenerateTerrain) {
        terrainSeed_++;
        setupTerrain();
    }

    if (shouldResetScene) {
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
    terrainRenderer_(terrainRenderer),
    terrainData_ {
        resolutionX_,
        resolutionZ_,
        terrainWorldSize_,
        terrainWorldPosition_,
        rainDropsAmount_
    } {}

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
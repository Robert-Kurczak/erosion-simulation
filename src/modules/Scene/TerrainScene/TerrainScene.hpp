#pragma once

#include "../IScene.hpp"
#include "InputController/IInputController.hpp"
#include "RainGenerator/IRainGenerator.hpp"
#include "TerrainData/TerrainData.hpp"
#include "TerrainGenerator/ITerrainGenerator.hpp"
#include "TerrainModifier/ITerrainModifier.hpp"
#include "TerrainRenderer/ITerrainRenderer.hpp"

#include <vector>

class TerrainScene : public IScene {
private:
    IInputController& inputController_;
    ITerrainGenerator& terrainGenerator_;
    IRainGenerator& rainGenerator_;
    const std::vector<ITerrainModifier*>& terrainModifiers_;
    ITerrainRenderer& terrainRenderer_;

    Camera3D mainCamera_ {};
    const float zoomSpeed_ = 10.0f;
    Vector3 lightSourcePosition_ {0.0f, 200.0f, 0.0f};

    const uint32_t resolutionX_ = 1024;
    const uint32_t resolutionZ_ = 1024;
    const Vector3 terrainWorldSize_ {256.0f, 128.0f, 256.0f};
    const Vector3 terrainWorldPosition_ {0.0f, 0.0f, 0.0f};
    const uint32_t rainDropsAmount_ = 300000;
    uint32_t terrainSeed_ = 20;
    TerrainData terrainData_;

    void setupCamera();
    void setupTerrain();
    void setCameraZoom(float zoom);

    void renderModels();
    void renderUi();
    void handleInput();

public:
    TerrainScene(
        IInputController& inputController,
        ITerrainGenerator& terrainGenerator,
        IRainGenerator& rainGenerator,
        const std::vector<ITerrainModifier*>& terrainModifiers,
        ITerrainRenderer& terrainRenderer
    );

    virtual void setup() override;
    virtual void draw() override;
};
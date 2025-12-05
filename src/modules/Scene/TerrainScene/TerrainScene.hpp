#pragma once

#include "../IScene.hpp"
#include "InputController/IInputController.hpp"
#include "Terrain/TerrainData.hpp"
#include "Terrain/TerrainModelConfig.hpp"
#include "TerrainGenerator/ITerrainGenerator.hpp"
#include "TerrainModifier/ITerrainModifier.hpp"
#include "TerrainRenderer/ITerrainRenderer.hpp"

class TerrainScene : public IScene {
private:
    IInputController& inputController_;
    ITerrainGenerator& terrainGenerator_;
    const std::vector<ITerrainModifier*>& terrainModifiers_;
    ITerrainRenderer& terrainRenderer_;

    Camera3D mainCamera_ {};
    const float zoomSpeed_ = 10.0f;
    TerrainData terrainData_;
    uint32_t terrainSeed_ = 20;
    TerrainModelConfig terrainModelConfig_ {
        .worldSize = {256.0f, 128.0f, 256.0f},
        .worldPosition = {0.0f,   0.0f,   0.0f  }
    };
    Vector3 lightSourcePosition_ {0.0f, 200.0f, 0.0f};

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
        const std::vector<ITerrainModifier*>& terrainModifiers,
        ITerrainRenderer& terrainRenderer
    );

    virtual void setup() override;
    virtual void draw() override;
};
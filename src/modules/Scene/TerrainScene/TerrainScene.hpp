#pragma once

#include "../IScene.hpp"
#include "Terrain/TerrainData.hpp"
#include "TerrainGenerator/ITerrainGenerator.hpp"
#include "TerrainModifier/ITerrainModifier.hpp"
#include "TerrainRenderer/ITerrainRenderer.hpp"

class TerrainScene : public IScene {
private:
    ITerrainGenerator& terrainGenerator_;
    const std::vector<ITerrainModifier*>& terrainModifiers_;
    ITerrainRenderer& terrainRenderer_;

    Camera3D mainCamera_ {};
    TerrainData terrainData_;
    Vector3 lightSourcePosition_ {0, 45, 0};

    void setupCamera();
    void setupTerrain();

public:
    TerrainScene(
        ITerrainGenerator& terrainGenerator,
        const std::vector<ITerrainModifier*>& terrainModifiers,
        ITerrainRenderer& terrainRenderer
    );

    virtual void setup() override;
    virtual void draw() override;
};
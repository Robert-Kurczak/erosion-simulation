#pragma once

#include "../IScene.hpp"
#include "Terrain/TerrainData.hpp"
#include "TerrainGenerator/ITerrainGenerator.hpp"
#include "TerrainPainter/ITerrainPainter.hpp"
#include "TerrainRenderer/ITerrainRenderer.hpp"

class TerrainScene : public IScene {
private:
    ITerrainGenerator& terrainGenerator_;
    ITerrainPainter& terrainPainter_;
    ITerrainRenderer& terrainRenderer_;

    Camera3D mainCamera_ {};
    TerrainData terrainData_;

    void setupCamera();
    void setupTerrain();

public:
    TerrainScene(
        ITerrainGenerator& terrainGenerator,
        ITerrainPainter& terrainPainter,
        ITerrainRenderer& terrainRenderer
    );

    virtual void setup() override;
    virtual void draw() override;
};
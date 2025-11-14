#pragma once

#include "../IScene.hpp"
#include "TerrainGenerator/ITerrainGenerator.hpp"

class TerrainScene : public IScene {
private:
    ITerrainGenerator& terrainGenerator_;

    Camera3D mainCamera_ {};
    Model terrainModel_ {};
    Vector3 terrainModelPosition_ {};

    void setupCamera();
    void setupTerrain();

public:
    TerrainScene(ITerrainGenerator& terrainGenerator);

    virtual void setup() override;
    virtual void draw() override;
};
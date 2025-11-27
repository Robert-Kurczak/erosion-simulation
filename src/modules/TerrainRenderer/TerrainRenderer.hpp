#pragma once

#include "ITerrainRenderer.hpp"
#include "MeshGenerator/IMeshGenerator.hpp"

#include <raylib.h>

class TerrainRenderer : public ITerrainRenderer {
private:
    IMeshGenerator& meshGenerator_;

    Mesh terrainMesh_;
    Model terrainModel_;
    Texture2D terrainTexture_;
    Vector3 terrainPosition_;
    Vector3 terrainWorldSize_;

    Image convertToTextureImage(const TerrainData& terrainData);
    Vector3 getCenteredPosition(const Model& model);

public:
    TerrainRenderer(IMeshGenerator& meshGenerator);

    virtual void setupModel(
        const TerrainData& terrainData,
        const TerrainModelConfig& config
    ) override;

    virtual void renderModel(const TerrainData& terrainData) override;
};

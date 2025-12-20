#pragma once

#include "ITerrainRenderer.hpp"
#include "MeshGenerator/IMeshGenerator.hpp"

class TerrainRenderer : public ITerrainRenderer {
private:
    IMeshGenerator& meshGenerator_;

    Mesh terrainMesh_;
    Model terrainModel_;
    Texture2D terrainTexture_;

    Image convertToTextureImage(const TerrainData& terrainData);

    void cleanup();

public:
    TerrainRenderer(IMeshGenerator& meshGenerator);

    virtual void setupModel(
        const TerrainData& terrainData,
        const Vector3& lightSource
    ) override;

    virtual void renderModel(
        const TerrainData& terrainData,
        const Vector3& lightSource
    ) override;
};

#pragma once

#include "ITerrainRenderer.hpp"

#include <raylib.h>

class TerrainRenderer : public ITerrainRenderer {
private:
    Model terrainModel_;
    Vector3 terrainPosition_;

    Image convertToHeightImage(const TerrainData& terrainData);
    Vector3 getCenteredPosition(const Model& model);

public:
    virtual void setupModel(
        const TerrainData& terrainData,
        const TerrainModelConfig& config
    ) override;
    virtual void renderModel(const TerrainData& terrainData) override;
};

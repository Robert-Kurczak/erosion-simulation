#pragma once

#include "../ITerrainModifier.hpp"
#include "RainGenerator/RainDrop.hpp"

#include <vector>

class HydraulicErosion : public ITerrainModifier {
private:
    Vector2 calculateGradient(
        const Vector2& position,
        const TerrainData& terrainData
    ) const;

    RainDrop getDerivatives(
        const Vector2& position,
        const Vector2& velocity,
        const TerrainData& terrainData
    );

    void integrateStepRK4(
        RainDrop& drop,
        const TerrainData& terrainData,
        double timeStep
    );

public:
    virtual void modify(TerrainData& terrainData) override;
};
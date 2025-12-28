#pragma once

#include "../ITerrainModifier.hpp"
#include "RandomNumberGenerator/IRandomNumberGenerator.hpp"
#include "WaterDroplet/WaterDroplet.hpp"

#include <vector>

class HydraulicErosion : public ITerrainModifier {
private:
    IRandomNumberGenerator& randomNumberGenerator_;

    Vector2 calculateGradient(
        const Vector2& position,
        const TerrainData& terrainData
    ) const;

    WaterDroplet getDerivatives(
        const Vector2& position,
        const Vector2& velocity,
        const TerrainData& terrainData
    );

    void integrateStepRK4(
        WaterDroplet& droplet,
        const TerrainData& terrainData,
        double timeStep
    );

public:
    HydraulicErosion(IRandomNumberGenerator& randomNumberGenerator);

    virtual void modify(TerrainData& terrainData) override;
};
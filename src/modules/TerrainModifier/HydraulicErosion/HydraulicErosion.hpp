#pragma once

#include "../ITerrainModifier.hpp"
#include "RainGenerator/RainDrop.hpp"
#include "RandomNumberGenerator/IRandomNumberGenerator.hpp"

#include <vector>

class HydraulicErosion : public ITerrainModifier {
private:
    IRandomNumberGenerator& randomNumberGenerator_;

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

    void resetDrop(const BoundingBox& boundingBox, RainDrop& drop);

public:
    HydraulicErosion(IRandomNumberGenerator& randomNumberGenerator);

    virtual void modify(TerrainData& terrainData) override;
};
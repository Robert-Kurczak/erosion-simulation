#pragma once

#include "../ITerrainModifier.hpp"
#include "RandomNumberGenerator/IRandomNumberGenerator.hpp"
#include "WaterDroplet/WaterDroplet.hpp"

#include <array>
#include <vector>

class HydraulicErosion : public ITerrainModifier {
private:
    static constexpr float EROSION_RATE_ = 0.03f;
    static constexpr float DEPOSITION_RATE_ = 0.75f;
    static constexpr float EVAPORATION_RATE_ = 0.05f;
    static constexpr float CAPACITY_FACTOR_ = 7.35f;

    static constexpr std::array<std::array<float, 5>, 5> BRUSH_WEIGHTS_ {
        {{{0.02, 0.04, 0.06, 0.04, 0.02}},
         {{0.04, 0.08, 0.12, 0.08, 0.04}},
         {{0.06, 0.12, 0.19, 0.12, 0.06}},
         {{0.04, 0.08, 0.12, 0.08, 0.04}},
         {{0.02, 0.04, 0.06, 0.04, 0.02}}}
    };

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

    WaterDroplet getNextDropletState(
        const WaterDroplet& droplet,
        const TerrainData& terrainData,
        float timeStep
    );

    void transformWithBrush(
        const Vector2& worldPosition,
        float delta,
        TerrainData& terrainData
    );

    void moveSedimentDown(
        const WaterDroplet& previousState,
        WaterDroplet& nextState,
        TerrainData& terrainData,
        float deltaHeight,
        float deltaTime
    );

    void moveSedimentUp(
        const WaterDroplet& previousState,
        WaterDroplet& nextState,
        TerrainData& terrainData,
        float deltaHeight
    );

    void transportSediment(
        const WaterDroplet& previousState,
        WaterDroplet& nextState,
        TerrainData& terrainData,
        float deltaTime
    );

public:
    HydraulicErosion(IRandomNumberGenerator& randomNumberGenerator);

    virtual void modify(TerrainData& terrainData) override;
};
#include "HydraulicErosion.hpp"

#include <functional>
#include <raymath.h>
#include <stdint.h>

Vector2 HydraulicErosion::calculateGradient(
    const Vector2& position,
    const TerrainData& terrainData
) const {
    // A------B
    // |----*-|
    // |------|
    // C------D
    // * - RainDrop on grid

    const double pointAHeight =
        terrainData.heightAt(uint32_t(position.x), uint32_t(position.y));
    const double pointBHeight = terrainData.heightAt(
        uint32_t(position.x + 1), uint32_t(position.y)
    );
    const double pointCHeight = terrainData.heightAt(
        uint32_t(position.x), uint32_t(position.y + 1)
    );
    const double pointDHeight = terrainData.heightAt(
        uint32_t(position.x + 1), uint32_t(position.y + 1)
    );

    const double slopeX1 = pointBHeight - pointAHeight;
    const double slopeX2 = pointDHeight - pointCHeight;
    const double slopeY1 = pointCHeight - pointAHeight;
    const double slopeY2 = pointDHeight - pointBHeight;

    const Vector2 dropCoordInCell {
        position.x - uint32_t(position.x),
        position.y - uint32_t(position.y)
    };

    const Vector2 gradient {
        float(
            slopeX1 * (1.0f - dropCoordInCell.x) +
            slopeX2 * (dropCoordInCell.x)
        ),
        float(
            slopeY1 * (1.0f - dropCoordInCell.y) +
            slopeY2 * (dropCoordInCell.y)
        )
    };

    return gradient;
}

RainDrop HydraulicErosion::getDerivatives(
    const Vector2& position,
    const Vector2& velocity,
    const TerrainData& terrainData
) {
    // Fg = m * a
    // Fg = -m * g * ∇h(x, z)
    // Fd = -v * n
    // F = Fg + Fd
    // F = -m * g * ∇h(x, z) - v * n
    // v = df(t)/dt
    // a = dv/dt

    // -m*g*∇h(x, z) - v * n = m * dv/dt

    // df(t)/dt = v
    // dv/dt = -g∇h(x, z) - v*n/m

    static const double g = 9.81;
    static const double n = 1;
    static const double m = 1;

    const Vector2 gradient = calculateGradient(position, terrainData);
    const Vector2 acceleration {
        float(-g * gradient.x - velocity.x * n / m),
        float(-g * gradient.y - velocity.y * n / m)
    };

    return RainDrop {
        .worldPosition = velocity,
        .velocity = acceleration,
        .deposition = 0.0f
    };
}

void HydraulicErosion::integrateStepRK4(
    RainDrop& drop,
    const TerrainData& terrainData,
    double timeStep
) {
    const RainDrop k1 =
        getDerivatives(drop.worldPosition, drop.velocity, terrainData);

    const RainDrop k2 = getDerivatives(
        drop.worldPosition + (k1.worldPosition * timeStep / 2.0f),
        drop.velocity + (k1.velocity * timeStep / 2.0f),
        terrainData
    );

    const RainDrop k3 = getDerivatives(
        drop.worldPosition + (k2.worldPosition * timeStep / 2.0f),
        drop.velocity + (k2.velocity * timeStep / 2.0f),
        terrainData
    );

    const RainDrop k4 = getDerivatives(
        drop.worldPosition + (k3.worldPosition * timeStep),
        drop.velocity + (k3.velocity * timeStep),
        terrainData
    );

    drop.worldPosition += (k1.worldPosition + (k2.worldPosition * 2) +
                           (k3.worldPosition * 2) + k4.worldPosition) *
                          (timeStep / 6.0f);

    drop.velocity += (k1.velocity + (k2.velocity * 2) +
                      (k3.velocity * 2) + k4.velocity) *
                     (timeStep / 6.0f);
}

void HydraulicErosion::modify(TerrainData& terrainData) {
    for (RainDrop& drop : terrainData.rainMap) {
        integrateStepRK4(drop, terrainData, GetFrameTime());
    }
}
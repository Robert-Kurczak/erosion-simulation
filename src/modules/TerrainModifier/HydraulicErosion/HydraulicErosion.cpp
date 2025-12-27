#include "HydraulicErosion.hpp"

#include <functional>
#include <iostream>
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

    const Vector2 indices = terrainData.worldPositionToIndices(position);
    const uint32_t x = indices.x;
    const uint32_t z = indices.y;

    const bool isXAtBorder =
        x <= 0 || x >= terrainData.getResolutionX() - 1;
    const bool isZAtBorder =
        z <= 0 || z >= terrainData.getResolutionZ() - 1;

    if (isXAtBorder || isZAtBorder) {
        return Vector2Zeros;
    }

    const double pointAHeight =
        terrainData.heightAt(x, z) * terrainData.getWorldSize().y;
    const double pointBHeight =
        terrainData.heightAt(x + 1, z) * terrainData.getWorldSize().y;
    const double pointCHeight =
        terrainData.heightAt(x, z + 1) * terrainData.getWorldSize().y;
    const double pointDHeight =
        terrainData.heightAt(x + 1, z + 1) * terrainData.getWorldSize().y;

    const double slopeX1 = pointBHeight - pointAHeight;
    const double slopeX2 = pointDHeight - pointCHeight;
    const double slopeY1 = pointCHeight - pointAHeight;
    const double slopeY2 = pointDHeight - pointBHeight;

    const Vector2 dropCoordInCell {
        position.x - int32_t(position.x), position.y - int32_t(position.y)
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
    static const double n = 0.85;
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

void HydraulicErosion::resetDrop(
    const BoundingBox& boundingBox,
    RainDrop& drop
) {
    drop.worldPosition.x = randomNumberGenerator_.getRandomFloat(
        boundingBox.min.x, boundingBox.max.x
    );
    drop.worldPosition.y = randomNumberGenerator_.getRandomFloat(
        boundingBox.min.z, boundingBox.max.z
    );
    drop.velocity = Vector2Zeros;
    drop.deposition = 0.0f;
}

HydraulicErosion::HydraulicErosion(
    IRandomNumberGenerator& randomNumberGenerator
) :
    randomNumberGenerator_(randomNumberGenerator) {}

void HydraulicErosion::modify(TerrainData& terrainData) {
    for (RainDrop& drop : terrainData.getRainMap()) {
        integrateStepRK4(drop, terrainData, GetFrameTime());

        const bool validPosition =
            terrainData.isInsideBoundingBox(drop.worldPosition);

        const bool stillMoving = Vector2Length(drop.velocity) > 0;

        if (!validPosition || !stillMoving) {
            resetDrop(terrainData.getBoundingBox(), drop);
        }

        terrainData.mutableColorAtWorld(drop.worldPosition) = RED;
    }
}
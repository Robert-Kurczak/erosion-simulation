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

WaterDroplet HydraulicErosion::getDerivatives(
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

    return WaterDroplet {velocity, acceleration, 0.0f, 0.0f};
}

void HydraulicErosion::integrateStepRK4(
    WaterDroplet& droplet,
    const TerrainData& terrainData,
    double timeStep
) {
    const WaterDroplet k1 = getDerivatives(
        droplet.getPosition(), droplet.getVelocity(), terrainData
    );

    const WaterDroplet k2 = getDerivatives(
        droplet.getPosition() + (k1.getPosition() * timeStep / 2.0f),
        droplet.getVelocity() + (k1.getVelocity() * timeStep / 2.0f),
        terrainData
    );

    const WaterDroplet k3 = getDerivatives(
        droplet.getPosition() + (k2.getPosition() * timeStep / 2.0f),
        droplet.getVelocity() + (k2.getVelocity() * timeStep / 2.0f),
        terrainData
    );

    const WaterDroplet k4 = getDerivatives(
        droplet.getPosition() + (k3.getPosition() * timeStep),
        droplet.getVelocity() + (k3.getVelocity() * timeStep),
        terrainData
    );

    const Vector2 deltaPosition =
        (k1.getPosition() + (k2.getPosition() * 2) +
         (k3.getPosition() * 2) + k4.getPosition()) *
        (timeStep / 6.0f);

    const Vector2 deltaVelocity =
        (k1.getVelocity() + (k2.getVelocity() * 2) +
         (k3.getVelocity() * 2) + k4.getVelocity()) *
        (timeStep / 6.0f);

    droplet.addPosition(deltaPosition);
    droplet.addVelocity(deltaVelocity);
}

HydraulicErosion::HydraulicErosion(
    IRandomNumberGenerator& randomNumberGenerator
) :
    randomNumberGenerator_(randomNumberGenerator) {}

void HydraulicErosion::modify(TerrainData& terrainData) {
    for (WaterDroplet& droplet : terrainData.getRainMap()) {
        integrateStepRK4(droplet, terrainData, GetFrameTime());

        const bool validPosition =
            terrainData.isInsideBoundingBox(droplet.getPosition());

        const bool stillMoving = Vector2Length(droplet.getVelocity()) > 0;

        if (!validPosition || !stillMoving) {
            droplet.randomizeParameters(
                randomNumberGenerator_, terrainData.getBoundingBox()
            );
        }

        terrainData.mutableColorAtWorld(droplet.getPosition()) = RED;
    }
}
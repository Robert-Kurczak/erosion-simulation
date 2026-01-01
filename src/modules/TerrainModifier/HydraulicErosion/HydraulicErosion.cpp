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
        x == 0 || x >= terrainData.getResolutionX() - 1;
    const bool isZAtBorder =
        z == 0 || z >= terrainData.getResolutionZ() - 1;

    if (isXAtBorder || isZAtBorder) {
        return Vector2Zeros;
    }

    const float cellSizeX =
        terrainData.getWorldSize().x / terrainData.getResolutionX();
    const float cellSizeZ =
        terrainData.getWorldSize().z / terrainData.getResolutionZ();

    const double pointAHeight =
        terrainData.heightAt(x, z) * terrainData.getWorldSize().y;
    const double pointBHeight =
        terrainData.heightAt(x + 1, z) * terrainData.getWorldSize().y;
    const double pointCHeight =
        terrainData.heightAt(x, z + 1) * terrainData.getWorldSize().y;
    const double pointDHeight =
        terrainData.heightAt(x + 1, z + 1) * terrainData.getWorldSize().y;

    const double slopeX1 = (pointBHeight - pointAHeight) / cellSizeX;
    const double slopeX2 = (pointDHeight - pointCHeight) / cellSizeX;
    const double slopeY1 = (pointCHeight - pointAHeight) / cellSizeZ;
    const double slopeY2 = (pointDHeight - pointBHeight) / cellSizeZ;

    const Vector2 floatIndices =
        terrainData.worldPositionToFloatIndices(position);

    const Vector2 dropCoordInCell {
        floatIndices.x - int32_t(floatIndices.x),
        floatIndices.y - int32_t(floatIndices.y)
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

WaterDroplet HydraulicErosion::getNextDropletState(
    const WaterDroplet& droplet,
    const TerrainData& terrainData,
    float timeStep
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

    const float waterToEvaporate =
        droplet.getWaterAmount() * EVAPORATION_RATE_ * timeStep;

    WaterDroplet nextState = droplet;
    nextState.addPosition(deltaPosition);
    nextState.addVelocity(deltaVelocity);
    nextState.addWater(-waterToEvaporate);

    return nextState;
}

void HydraulicErosion::transformWithBrush(
    const Vector2& worldPosition,
    float delta,
    TerrainData& terrainData
) {
    const Vector2 indices =
        terrainData.worldPositionToIndices(worldPosition);
    const uint32_t x = indices.x;
    const uint32_t z = indices.y;

    const int8_t brushRadius = BRUSH_WEIGHTS_.size() / 2;

    for (int8_t brushZ = -brushRadius; brushZ < brushRadius; brushZ++) {
        for (int8_t brushX = -brushRadius; brushX < brushRadius;
             brushX++) {
            const uint32_t neighborX = x + brushX;
            const uint32_t neighborZ = z + brushZ;

            if (!terrainData.isInsideHeightMap(neighborX, neighborZ)) {
                continue;
            }

            terrainData.mutableHeightAt(neighborX, neighborZ) +=
                delta * BRUSH_WEIGHTS_.at(brushZ + brushRadius)
                            .at(brushX + brushRadius);
        }
    }
}

void HydraulicErosion::moveSedimentDown(
    const WaterDroplet& previousState,
    WaterDroplet& nextState,
    TerrainData& terrainData,
    float deltaHeight,
    float deltaTime
) {
    const float capacity = -deltaHeight *
                           Vector2Length(nextState.getVelocity()) *
                           nextState.getWaterAmount() * CAPACITY_FACTOR_;

    if (nextState.getSedimentAmount() > capacity) {
        const float sedimentToDeposit =
            (nextState.getSedimentAmount() - capacity) *
            DEPOSITION_RATE_ * deltaTime;

        transformWithBrush(
            previousState.getPosition(), sedimentToDeposit, terrainData
        );

        nextState.addSediment(-sedimentToDeposit);
    } else {
        float sedimentToErode =
            (capacity - nextState.getSedimentAmount()) * EROSION_RATE_ *
            deltaTime;

        sedimentToErode = Clamp(sedimentToErode, 0.0f, -deltaHeight);

        transformWithBrush(
            previousState.getPosition(), -sedimentToErode, terrainData
        );

        nextState.addSediment(sedimentToErode);
    }
}

void HydraulicErosion::moveSedimentUp(
    const WaterDroplet& previousState,
    WaterDroplet& nextState,
    TerrainData& terrainData,
    float deltaHeight
) {
    const float sedimentToDeposit =
        Clamp(nextState.getSedimentAmount(), 0, deltaHeight);

    transformWithBrush(
        previousState.getPosition(), sedimentToDeposit, terrainData
    );

    nextState.addSediment(-sedimentToDeposit);
}

void HydraulicErosion::transportSediment(
    const WaterDroplet& previousState,
    WaterDroplet& nextState,
    TerrainData& terrainData,
    float deltaTime
) {
    const double previousHeight =
        terrainData.heightAtWorld(previousState.getPosition());

    const double nextHeight =
        terrainData.heightAtWorld(nextState.getPosition());

    const double deltaHeight = nextHeight - previousHeight;

    if (deltaHeight < 0) {
        moveSedimentDown(
            previousState, nextState, terrainData, deltaHeight, deltaTime
        );
    } else {
        moveSedimentUp(
            previousState, nextState, terrainData, deltaHeight
        );
    }
}

void HydraulicErosion::drawDroplet(
    const WaterDroplet& droplet,
    TerrainData& terrainData
) {
    const Color dropletColor {60, 139, 156, 80};
    Color& currentColor =
        terrainData.mutableColorAtWorld(droplet.getPosition());

    currentColor = ColorAlphaBlend(currentColor, dropletColor, WHITE);
}

HydraulicErosion::HydraulicErosion(
    IRandomNumberGenerator& randomNumberGenerator,
    IInputController& inputController
) :
    randomNumberGenerator_(randomNumberGenerator),
    inputController_(inputController) {}

void HydraulicErosion::modify(TerrainData& terrainData) {
    if (inputController_.isActionReleased(InputAction::DrawRain)) {
        drawRain = !drawRain;
    }

    const float deltaTime = GetFrameTime();

    for (WaterDroplet& droplet : terrainData.getRainMap()) {
        WaterDroplet nextState =
            getNextDropletState(droplet, terrainData, deltaTime);

        const bool validPosition =
            terrainData.isInsideBoundingBox(nextState.getPosition());

        const bool stillMoving =
            Vector2LengthSqr(nextState.getVelocity()) > 1.0f;

        const bool haveWater = nextState.getWaterAmount() > 0.1f;

        if (validPosition && stillMoving && haveWater) {
            transportSediment(droplet, nextState, terrainData, deltaTime);
            droplet = nextState;
        } else {
            droplet.randomizeParameters(
                randomNumberGenerator_, terrainData.getBoundingBox()
            );
        }

        if (drawRain) {
            drawDroplet(droplet, terrainData);
        }
    }
}
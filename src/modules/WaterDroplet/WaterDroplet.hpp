#pragma once

#include "RandomNumberGenerator/IRandomNumberGenerator.hpp"

#include <raylib.h>
#include <raymath.h>

class WaterDroplet {
private:
    Vector2 position_ = Vector2Zeros;
    Vector2 velocity_ = Vector2Zeros;
    float waterAmount_ = 0.0f;
    float sedimentAmount_ = 0.0f;

public:
    WaterDroplet(
        const Vector2& position,
        const Vector2& velocity,
        float waterAmount,
        float sedimentAmount
    );

    WaterDroplet();

    const Vector2& getPosition() const;
    const Vector2& getVelocity() const;
    float getWaterAmount() const;
    float getSedimentAmount() const;

    void addPosition(const Vector2& delta);
    void addVelocity(const Vector2& delta);
    void addWater(float delta);
    void addSediment(float delta);

    void randomizePosition(
        IRandomNumberGenerator& randomNumberGenerator,
        const BoundingBox& boundingBox
    );

    void randomizeVelocity(
        IRandomNumberGenerator& randomNumberGenerator,
        const Vector2& minVelocity,
        const Vector2& maxVelocity
    );

    void randomizeWaterAmount(
        IRandomNumberGenerator& randomNumberGenerator
    );

    void randomizeParameters(
        IRandomNumberGenerator& randomNumberGenerator,
        const BoundingBox& boundingBox,
        const Vector2& minVelocity,
        const Vector2& maxVelocity
    );

    void randomizeParameters(
        IRandomNumberGenerator& randomNumberGenerator,
        const BoundingBox& boundingBox
    );
};
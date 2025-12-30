#include "WaterDroplet.hpp"

#include <raymath.h>

WaterDroplet::WaterDroplet(
    const Vector2& position,
    const Vector2& velocity,
    float waterAmount,
    float sedimentAmount
) :
    position_(position),
    velocity_(velocity),
    waterAmount_(waterAmount),
    sedimentAmount_(sedimentAmount) {}

WaterDroplet::WaterDroplet() {}

const Vector2& WaterDroplet::getPosition() const {
    return position_;
}

const Vector2& WaterDroplet::getVelocity() const {
    return velocity_;
}

float WaterDroplet::getWaterAmount() const {
    return waterAmount_;
}

float WaterDroplet::getSedimentAmount() const {
    return sedimentAmount_;
}

void WaterDroplet::addPosition(const Vector2& delta) {
    position_.x += delta.x;
    position_.y += delta.y;
}

void WaterDroplet::addVelocity(const Vector2& delta) {
    velocity_.x += delta.x;
    velocity_.y += delta.y;
}

void WaterDroplet::addWater(float delta) {
    waterAmount_ += delta;
}

void WaterDroplet::addSediment(float delta) {
    sedimentAmount_ += delta;
}

void WaterDroplet::randomizePosition(
    IRandomNumberGenerator& randomNumberGenerator,
    const BoundingBox& boundingBox
) {
    position_.x = randomNumberGenerator.getRandomFloat(
        boundingBox.min.x, boundingBox.max.x
    );
    position_.y = randomNumberGenerator.getRandomFloat(
        boundingBox.min.z, boundingBox.max.z
    );
}

void WaterDroplet::randomizeVelocity(
    IRandomNumberGenerator& randomNumberGenerator,
    const Vector2& minVelocity,
    const Vector2& maxVelocity
) {
    velocity_.x = randomNumberGenerator.getRandomFloat(
        minVelocity.x, maxVelocity.x
    );
    velocity_.y = randomNumberGenerator.getRandomFloat(
        minVelocity.y, maxVelocity.y
    );
}

void WaterDroplet::randomizeWaterAmount(
    IRandomNumberGenerator& randomNumberGenerator
) {
    waterAmount_ = randomNumberGenerator.getRandomFloat();
}

void WaterDroplet::randomizeParameters(
    IRandomNumberGenerator& randomNumberGenerator,
    const BoundingBox& boundingBox,
    const Vector2& minVelocity,
    const Vector2& maxVelocity
) {
    randomizePosition(randomNumberGenerator, boundingBox);
    randomizeVelocity(randomNumberGenerator, minVelocity, maxVelocity);
    randomizeWaterAmount(randomNumberGenerator);
}

void WaterDroplet::randomizeParameters(
    IRandomNumberGenerator& randomNumberGenerator,
    const BoundingBox& boundingBox
) {
    randomizePosition(randomNumberGenerator, boundingBox);
    randomizeVelocity(
        randomNumberGenerator, {-0.5f, -0.5f}, {0.5f, 0.5f}
    );
    randomizeWaterAmount(randomNumberGenerator);
}
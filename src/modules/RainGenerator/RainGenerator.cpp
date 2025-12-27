#include "RainGenerator.hpp"

RainGenerator::RainGenerator(IRandomNumberGenerator& randomNumberGenerator
) :
    randomNumberGenerator_(randomNumberGenerator) {}

void RainGenerator::generateRainDrops(
    uint32_t dropsAmount,
    uint32_t seed,
    const BoundingBox& boundingBox,
    std::span<RainDrop> outputBuffer
) {
    randomNumberGenerator_.setSeed(seed);

    for (uint32_t i = 0; i < dropsAmount; i++) {
        const Vector2 position {
            randomNumberGenerator_.getRandomFloat(
                boundingBox.min.x, boundingBox.max.x
            ),
            randomNumberGenerator_.getRandomFloat(
                boundingBox.min.z, boundingBox.max.z
            ),
        };

        const Vector2 velocity {0, 0};

        const RainDrop rainDrop {
            .worldPosition = position,
            .velocity = velocity,
            .deposition = 0.0f
        };

        outputBuffer[i] = rainDrop;
    }
}
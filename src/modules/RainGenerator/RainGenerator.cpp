#include "RainGenerator.hpp"

#include <FastNoiseLite.h>

std::vector<RainDrop> RainGenerator::generateRainDrops(
    const Vector3& worldArea,
    uint32_t dropsAmount,
    uint32_t seed
) {
    FastNoiseLite noiseGenerator {};
    noiseGenerator.SetSeed(seed);

    std::vector<RainDrop> rainDrops(dropsAmount);

    for (uint32_t i = 0; i < dropsAmount; i++) {
        const float randomX =
            (noiseGenerator.GetNoise(float(i), 0.0f) + 1.0) / 2.0f;

        const float randomZ =
            (noiseGenerator.GetNoise(0.0f, float(i)) + 1.0) / 2.0f;

        const Vector2 position {
            randomX * worldArea.x, randomZ * worldArea.z
        };

        const Vector2 velocity {0, 0};

        const RainDrop rainDrop {
            .worldPosition = position,
            .velocity = velocity,
            .deposition = 0.0f
        };

        rainDrops.push_back(rainDrop);
    }

    return rainDrops;
}
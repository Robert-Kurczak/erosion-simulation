#include "RainGenerator.hpp"

RainGenerator::RainGenerator(IRandomNumberGenerator& randomNumberGenerator
) :
    randomNumberGenerator_(randomNumberGenerator) {}

void RainGenerator::generateRainDrops(
    const BoundingBox& boundingBox,
    std::span<WaterDroplet> outputBuffer
) {
    for (uint32_t i = 0; i < outputBuffer.size(); i++) {
        outputBuffer[i].randomizeParameters(
            randomNumberGenerator_, boundingBox
        );
    }
}
#pragma once

#include "WaterDroplet/WaterDroplet.hpp"

#include <raylib.h>
#include <span>
#include <stdint.h>

class IRainGenerator {
public:
    virtual ~IRainGenerator() = default;

    virtual void generateRainDrops(
        const BoundingBox& boundingBox,
        std::span<WaterDroplet> outputBuffer
    ) = 0;
};
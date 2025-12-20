#pragma once

#include "RainDrop.hpp"

#include <raylib.h>
#include <span>
#include <stdint.h>

class IRainGenerator {
public:
    virtual ~IRainGenerator() = default;

    virtual void generateRainDrops(
        const Vector3& worldArea,
        uint32_t dropsAmount,
        uint32_t seed,
        std::span<RainDrop> outputBuffer
    ) = 0;
};
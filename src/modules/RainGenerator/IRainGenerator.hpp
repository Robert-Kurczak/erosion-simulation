#pragma once

#include "RainDrop.hpp"

#include <raylib.h>
#include <stdint.h>
#include <vector>

class IRainGenerator {
public:
    virtual ~IRainGenerator() = default;

    virtual std::vector<RainDrop> generateRainDrops(
        const Vector3& worldArea,
        uint32_t dropsAmount,
        uint32_t seed
    ) = 0;
};
#pragma once

#include "IRainGenerator.hpp"

class RainGenerator : public IRainGenerator {
public:
    virtual std::vector<RainDrop> generateRainDrops(
        const Vector3& worldArea,
        uint32_t dropsAmount,
        uint32_t seed
    ) override;
};
#pragma once

#include <stdint.h>
#include <string>

struct ApplicationConfig {
    std::string name;
    float windowWidthScale;
    float windowHeightScale;
    uint8_t framesPerSeconds;
};
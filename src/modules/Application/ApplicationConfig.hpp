#pragma once

#include <stdint.h>
#include <string>

struct ApplicationConfig {
    std::string name;
    uint32_t widthInPixels;
    uint32_t heightInPixels;
    uint8_t framesPerSeconds;
};
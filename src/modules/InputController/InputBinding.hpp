#pragma once

#include "InputSource.hpp"

#include <stdint.h>

struct InputBinding {
    InputSource source;
    uint32_t key;
};
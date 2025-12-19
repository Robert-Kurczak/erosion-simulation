#pragma once

#include <raylib.h>

struct RainDrop {
    Vector2 worldPosition;
    Vector2 velocity;
    float deposition;
};
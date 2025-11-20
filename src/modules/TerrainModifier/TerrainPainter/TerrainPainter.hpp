#pragma once

#include "../ITerrainModifier.hpp"

#include <array>
#include <raylib.h>

class TerrainPainter : public ITerrainModifier {
private:
    static constexpr std::array<Color, 24> COLOR_GRADIENT_ {
        Color {47,  77,  84,  255},
         Color {60,  139, 156, 255},
        Color {71,  188, 213, 255},
         Color {170, 209, 135, 255},
        Color {250, 225, 72,  255},
         Color {221, 222, 70,  255},
        Color {192, 219, 68,  255},
         Color {169, 217, 67,  255},
        Color {150, 197, 50,  255},
         Color {131, 176, 33,  255},
        Color {112, 156, 16,  255},
         Color {97,  140, 3,   255},
        Color {100, 136, 19,  255},
         Color {103, 133, 36,  255},
        Color {106, 129, 52,  255},
         Color {109, 126, 68,  255},
        Color {112, 122, 85,  255},
         Color {115, 119, 101, 255},
        Color {117, 116, 114, 255},
         Color {180, 179, 178, 255},
        Color {242, 242, 242, 255},
         Color {242, 242, 242, 255},
        Color {242, 242, 242, 255},
    };

    Color convertHeightToColor(double height);

public:
    virtual void modify(TerrainData& terrainData) override;
};
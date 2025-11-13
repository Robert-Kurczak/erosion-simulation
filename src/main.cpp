#include "modules/Application/Application.hpp"
#include "modules/Application/ApplicationConfig.hpp"
#include "raylib.h"

ApplicationConfig applicationConfig_ {
    .name = "Erosion Simulator",
    .widthInPixels = 1280,
    .heightInPixels = 720,
    .framesPerSeconds = 120
};

Application application_ {applicationConfig_};

int main(void) {
    application_.setup();
    application_.enterMainLoop();

    return 0;
}
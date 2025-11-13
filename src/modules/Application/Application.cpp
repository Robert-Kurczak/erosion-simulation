#include "Application.hpp"

#include "raylib.h"

Application::Application(ApplicationConfig config) :
    applicationConfig_(config) {}

void Application::setup() {
    InitWindow(
        applicationConfig_.widthInPixels,
        applicationConfig_.heightInPixels,
        applicationConfig_.name.c_str()
    );

    SetTargetFPS(applicationConfig_.framesPerSeconds);
}

void Application::enterMainLoop() {
    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawRectangle(
            applicationConfig_.widthInPixels / 4,
            applicationConfig_.heightInPixels / 4,
            applicationConfig_.widthInPixels / 2,
            applicationConfig_.heightInPixels / 2,
            RED
        );

        EndDrawing();
    }

    CloseWindow();
}
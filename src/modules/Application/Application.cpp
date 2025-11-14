#include "Application.hpp"

#include <iostream>
#include <raylib.h>

void Application::setupWindow() {
    InitWindow(0, 0, applicationConfig_.name.c_str());

    const uint8_t monitorId = GetCurrentMonitor();
    const uint32_t monitorWidth = GetMonitorWidth(monitorId);
    const uint32_t monitorHeight = GetMonitorHeight(monitorId);

    const uint32_t windowWidth =
        monitorWidth * applicationConfig_.windowWidthScale;

    const uint32_t windowHeight =
        monitorHeight * applicationConfig_.windowHeightScale;

    SetWindowSize(windowWidth, windowHeight);
    SetWindowPosition(
        (monitorWidth - windowWidth) / 2,
        (monitorHeight - windowHeight) / 2
    );
    SetTargetFPS(applicationConfig_.framesPerSeconds);
}

Application::Application(ApplicationConfig config, IScene& scene) :
    applicationConfig_(config),
    scene_(scene) {}

void Application::setup() {
    setupWindow();
    scene_.setup();
}

void Application::enterMainLoop() {
    while (!WindowShouldClose()) {
        scene_.draw();
    }

    CloseWindow();
}
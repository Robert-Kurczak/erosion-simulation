#pragma once

#include "ApplicationConfig.hpp"
#include "IApplication.hpp"
#include "Scene/IScene.hpp"

class Application : public IApplication {
private:
    const ApplicationConfig applicationConfig_;
    IScene& scene_;

    void setupWindow();

public:
    Application(ApplicationConfig config, IScene& scene);

    virtual void setup() override;
    virtual void enterMainLoop() override;
};
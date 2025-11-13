#pragma once

#include "ApplicationConfig.hpp"
#include "IApplication.hpp"

class Application : public IApplication {
private:
    const ApplicationConfig applicationConfig_;

public:
    Application(ApplicationConfig config);

    virtual void setup() override;
    virtual void enterMainLoop() override;
};
#pragma once

#include "ApplicationConfig.hpp"

class IApplication {
public:
    virtual ~IApplication() = default;

    virtual void setup() = 0;
    virtual void enterMainLoop() = 0;
};
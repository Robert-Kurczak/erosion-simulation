#pragma once
#include "InputAction.hpp"

class IInputController {
public:
    virtual ~IInputController() = default;

    virtual bool isActionDown(InputAction action) const = 0;
    virtual bool isActionUp(InputAction action) const = 0;
    virtual bool isActionPressed(InputAction action) const = 0;
    virtual bool isActionReleased(InputAction action) const = 0;
    virtual float getActionValue(InputAction action) const = 0;
};
#pragma once
#include "InputAction.hpp"
#include "InputBinding.hpp"

#include <unordered_map>

class IInputController {
public:
    virtual ~IInputController() = default;

    virtual bool isActionDown(InputAction action) const = 0;
    virtual bool isActionUp(InputAction action) const = 0;
    virtual bool isActionPressed(InputAction action) const = 0;
    virtual bool isActionReleased(InputAction action) const = 0;
    virtual float getActionValue(InputAction action) const = 0;

    virtual const std::unordered_map<InputAction, InputBinding>&
    getBindings() const = 0;
    virtual std::string_view getActionName(InputAction action) const = 0;
    virtual std::string_view getBindingName(const InputBinding& binding
    ) const = 0;
};
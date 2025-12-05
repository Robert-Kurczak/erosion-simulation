#include "KeyboardInputController.hpp"

#include <raylib.h>

const std::unordered_map<InputAction, InputBinding>
    KeyboardInputController::ACTION_BINDINGS_ {
        {InputAction::RegenerateTerrain, {InputSource::Keyboard, KEY_R}     },
        {InputAction::LookAround,
         {InputSource::MouseButton, MOUSE_BUTTON_LEFT}                      },
        {InputAction::CameraZoom,        {InputSource::MouseWheel, KEY_NULL}},
};

bool KeyboardInputController::isActionDown(InputAction action) const {
    const InputBinding binding = ACTION_BINDINGS_.at(action);

    if (binding.source == InputSource::Keyboard) {
        return IsKeyDown(binding.key);
    }

    if (binding.source == InputSource::MouseButton) {
        return IsMouseButtonDown(binding.key);
    }

    return false;
}

bool KeyboardInputController::isActionUp(InputAction action) const {
    const InputBinding binding = ACTION_BINDINGS_.at(action);

    if (binding.source == InputSource::Keyboard) {
        return IsKeyUp(binding.key);
    }

    if (binding.source == InputSource::MouseButton) {
        return IsMouseButtonUp(binding.key);
    }

    return false;
}

bool KeyboardInputController::isActionPressed(InputAction action) const {
    const InputBinding binding = ACTION_BINDINGS_.at(action);

    if (binding.source == InputSource::Keyboard) {
        return IsKeyPressed(binding.key);
    }

    if (binding.source == InputSource::MouseButton) {
        return IsMouseButtonPressed(binding.key);
    }

    return false;
}

bool KeyboardInputController::isActionReleased(InputAction action) const {
    const InputBinding binding = ACTION_BINDINGS_.at(action);

    if (binding.source == InputSource::Keyboard) {
        return IsKeyReleased(binding.key);
    }

    if (binding.source == InputSource::MouseButton) {
        return IsMouseButtonReleased(binding.key);
    }

    return false;
}

float KeyboardInputController::getActionValue(InputAction action) const {
    const InputBinding binding = ACTION_BINDINGS_.at(action);

    if (binding.source == InputSource::MouseWheel) {
        return GetMouseWheelMove();
    }

    return 0.0f;
}

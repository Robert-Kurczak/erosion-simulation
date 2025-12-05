#include "IInputController.hpp"
#include "InputBinding.hpp"

#include <stdint.h>
#include <unordered_map>

class KeyboardInputController : public IInputController {
private:
    static const std::unordered_map<InputAction, InputBinding>
        ACTION_BINDINGS_;

public:
    virtual bool isActionDown(InputAction action) const override;
    virtual bool isActionUp(InputAction action) const override;
    virtual bool isActionPressed(InputAction action) const override;
    virtual bool isActionReleased(InputAction action) const override;
    virtual float getActionValue(InputAction action) const override;
};
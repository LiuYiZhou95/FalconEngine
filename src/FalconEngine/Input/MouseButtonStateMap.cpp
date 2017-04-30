#include <FalconEngine/Input/MouseButtonState.h>

#include <vector>

namespace FalconEngine
{

MouseButtonState::MouseButtonState(MouseButton button) :
    mButton(button),
    mPressed(false),
    mDown(false),
    mUp(false)
{
}

MouseButtonStateMap::MouseButtonStateMap()
{
    static std::vector<MouseButton> sButtons = { MouseButton::LeftButton, MouseButton::RightButton, MouseButton::MiddleButton };

    int buttonNum = int(sButtons.size());
    for (int buttonIndex = 0; buttonIndex < buttonNum; ++buttonIndex)
    {
        auto button = sButtons[buttonIndex];
        insert({ button, MouseButtonState(button) });
    }
}

}

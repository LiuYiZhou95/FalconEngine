#pragma once

#include <FalconEngine/InputInclude.h>
#include <FalconEngine/Input/MouseButtonState.h>
#include <FalconEngine/Math/Vector2.h>

namespace FalconEngine
{

enum class MouseWheelDirection
{
    None = 0,
    Down = 1,
    Up = 2
};

class MouseState sealed
{
public:
    MouseState();
    ~MouseState() = default;

public:
    bool
    ButtonPressed(MouseButton button) const;

    bool
    ButtonReleased(MouseButton button) const;

    bool
    ButtonDown(MouseButton button) const;

    bool
    ButtonUp(MouseButton button) const;

    Vector2f
    GetPosition() const;

    void
    UpdatePosition(Vector2f mousePositionPrevious, Vector2f mousePositionCurrent);

    Vector2f
    GetPositionDiff() const;

    int
    GetWheelValue() const;

    int
    GetWheelValueDiff() const;
    void UpdateWheelValue(int wheelValueCurrent, int wheelValuePrevious);

internal:
    void
    SetButtonInternal(MouseButton button, bool pressed, double time);

    void
    SetPositionInternal(double x, double y, double time);

    void
    SetWheelValueInternal(double yoffset, double time);

    void
    UpdateEvent();

private:
    MouseButtonStateMap mButtonTable;
    Vector2f            mPosition;
    bool                mPositionChanged = false;
    Vector2f            mPositionDiff;

    MouseWheelDirection mWheelDirection = MouseWheelDirection::None;
    int                 mWheelValue;
    bool                mWheelValueChanged = false;
    int                 mWheelValueDiff;
};

}

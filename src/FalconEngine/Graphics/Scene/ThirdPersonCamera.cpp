#include <FalconEngine/Graphics/Scene/ThirdPersonCamera.h>

#include <FalconEngine/Context/GameEngineGraphics.h>
#include <FalconEngine/Context/GameEngineInput.h>
#include <FalconEngine/Input/KeyboardState.h>
#include <FalconEngine/Input/KeyState.h>
#include <FalconEngine/Input/MouseState.h>

namespace FalconEngine
{

FALCON_ENGINE_RTTI_IMPLEMENT(ThirdPersonCamera, Camera);

ThirdPersonCamera::ThirdPersonCamera(const Handedness *handedness) :
    PlayerCamera(handedness)
{
    Reset();
}

ThirdPersonCamera::ThirdPersonCamera(const Handedness *handedness, const Viewport& viewport, float nearPlane, float farPlane) :
    PlayerCamera(handedness, viewport, nearPlane, farPlane)
{
    Reset();
}

ThirdPersonCamera::ThirdPersonCamera(const Handedness *handedness, float fovy, float aspectRatio, float nearPlane, float farPlane) :
    PlayerCamera(handedness, fovy, aspectRatio, nearPlane, farPlane)
{
    Reset();
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
void
ThirdPersonCamera::SetTarget(Vector3f target)
{
    mOrigin = target;
}

void
ThirdPersonCamera::PanLeft(float distance)
{
    // NOTE(Wuxiang): Pan operation move the camera in opposite direction.
    mOrigin += -GetLeft() * distance;
}

void
ThirdPersonCamera::PanRight(float distance)
{
    // NOTE(Wuxiang): Pan operation move the camera in opposite direction.
    mOrigin += -GetRight() * distance;
}

void
ThirdPersonCamera::PanUp(float distance)
{
    // NOTE(Wuxiang): Pan operation move the camera in opposite direction.
    mOrigin += -GetUp() * distance;
}

void
ThirdPersonCamera::PanDown(float distance)
{
    // NOTE(Wuxiang): Pan operation move the camera in opposite direction.
    mOrigin += -GetDown() * distance;
}

void
ThirdPersonCamera::Update(GameEngineInput *input, double elapsed)
{
    auto tSecond = elapsed / 1000;

    auto mouse = input->GetMouseState();
    auto mousePositionDiff = mouse->GetPositionDiff();
    auto keyboard = input->GetKeyboardState();

    // Update camera control scheme (Blender style control scheme).
    {
        // TODO(Wuxiang): 2017-05-16 15:35 Add keyboard mapping support.
        auto keyCtrlPressed = keyboard->KeyPressed(Key::LeftControl) || keyboard->KeyPressed(Key::RightControl);
        auto keyShiftPressed = keyboard->KeyPressed(Key::LeftShift) || keyboard->KeyPressed(Key::RightShift);
        auto middleButtonPressed = mouse->ButtonPressed(MouseButton::MiddleButton);

        if (keyShiftPressed && middleButtonPressed)
        {
            mMode = ThirdPersonCameraMode::Pan;
        }
        else if (keyCtrlPressed && middleButtonPressed)
        {
            mMode = ThirdPersonCameraMode::Zoom;
        }
        else if (middleButtonPressed)
        {
            mMode = ThirdPersonCameraMode::Rotate;
        }
        else
        {
            mMode = ThirdPersonCameraMode::None;
        }
    }

    // Update camera position and orientation based on modes.
    switch (mMode)
    {
    case ThirdPersonCameraMode::None:
        break;

    case ThirdPersonCameraMode::Pan:
    {
        Vector2f panDistancePixel = mousePositionDiff * mMouseSensitivity * mMouseSensitivityAdjust
                                    * mPanSpeed * float(tSecond);

        // Pan distance in window space.
        Vector3f panDistanceWindow = Vector3f(panDistancePixel, 0);

        auto graphics = GameEngineGraphics::GetInstance();
        auto viewport = graphics->GetViewport();

        // Target position in window space.
        Vector3f targetPositionWindow = viewport->Project(mOrigin, mProjection, mView);

        // Target modified position in window space. Because we don't move the
        // objects in world space, we need to reverse the transform to move the
        // camera itself. Hence we use the - sign.
        Vector3f targetPositionFinalWindow = targetPositionWindow - panDistanceWindow;

        mOrigin = viewport->Unproject(targetPositionFinalWindow, mProjection, mView);

        auto z = mRadialDistance * glm::sin(mAzimuthalRadian) * glm::cos(mPolarRadian);
        auto x = mRadialDistance * glm::sin(mAzimuthalRadian) * glm::sin(mPolarRadian);
        auto y = mRadialDistance * glm::cos(mAzimuthalRadian);
        auto r = Vector3f(x, y, z);
        mPosition = mOrigin + r;

        break;
    }

    case ThirdPersonCameraMode::Rotate:
    {
        static auto azimuthalDegreeRotationPerSecondMax = 360 / 0.8;
        static auto polarDegreeRotationPerSecondMax = 360 / 0.8;

        auto azimuthalDegreeRotation = float(mousePositionDiff.y * mMouseSensitivity * mMouseSensitivityAdjust * azimuthalDegreeRotationPerSecondMax * tSecond);
        auto polarDegreeRotation = -float(mousePositionDiff.x * mMouseSensitivity * mMouseSensitivityAdjust * polarDegreeRotationPerSecondMax * tSecond);

        auto azimuthalDegreePrevious = Degree(mAzimuthalRadian);
        auto azimuthalDegree = azimuthalDegreePrevious + azimuthalDegreeRotation;
        azimuthalDegree = DegreeNormalize<float>(azimuthalDegree, 0, 180);
        mAzimuthalRadian = Radian(azimuthalDegree);

        auto polarDegreePrevious = Degree(mPolarRadian);
        auto polarDegree = polarDegreePrevious + polarDegreeRotation;
        polarDegree = DegreeNormalize<float>(polarDegree, -180, 180);
        mPolarRadian = Radian(polarDegree);

        // NOTE(Wuxiang): The spherical coordinate computation here won't be affected by
        // the sign of azimuthal angle and polar angle. Regardless of the sign of angles,
        // the computation yield correct r vector pointed from origin to target.
        auto z = mRadialDistance * glm::sin(mAzimuthalRadian) * glm::cos(mPolarRadian);
        auto x = mRadialDistance * glm::sin(mAzimuthalRadian) * glm::sin(mPolarRadian);
        auto y = mRadialDistance * glm::cos(mAzimuthalRadian);
        auto r = Vector3f(x, y, z);
        mPosition = mOrigin + r;

        auto pitchRadian = abs(mAzimuthalRadian) - PiOver2;
        auto pitchQuaternion = Quaternion::CreateFromAxisAngle(Vector3f::UnitX, pitchRadian);
        auto yawRadian = mPolarRadian;
        auto yawQuaternion = Quaternion::CreateFromAxisAngle(Vector3f::UnitY, yawRadian);
        auto pitchYawQuaternion = yawQuaternion * pitchQuaternion;

        // NOTE(Wuxiang): Inspired by NumberXaero's answer
        // https://www.gamedev.net/topic/653628-quaternion-camera-performs-unwanted-roll/
        auto forward = GetForward() * pitchYawQuaternion;
        auto rollQuaternion = Quaternion::CreateFromAxisAngle(forward, 0);

        SetOrientation(rollQuaternion * pitchYawQuaternion);

        break;
    }

    case ThirdPersonCameraMode::Zoom:
    {
        auto zoomDistance = mousePositionDiff.y * mMouseSensitivity * mMouseSensitivityAdjust
                            * mZoomSpeed * float(tSecond);
        mRadialDistance += zoomDistance;
        if (mRadialDistance < 0)
        {
            mRadialDistance = 0;
        }

        auto z = mRadialDistance * glm::sin(mAzimuthalRadian) * glm::cos(mPolarRadian);
        auto x = mRadialDistance * glm::sin(mAzimuthalRadian) * glm::sin(mPolarRadian);
        auto y = mRadialDistance * glm::cos(mAzimuthalRadian);
        auto r = Vector3f(x, y, z);
        mPosition = mOrigin + r;

        break;
    }

    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }

    Camera::Update(elapsed);
}

void
ThirdPersonCamera::Reset()
{
    mAzimuthalRadian = 0.0f;
    mPolarRadian = 0.0f;
    mRadialDistance = 0.0f;

    mOrigin = Vector3f::Zero;

    mPanSpeed = 500.0f;
    mRotateSpeed = 1.0f;
    mZoomSpeed = 100.0f;
}

}
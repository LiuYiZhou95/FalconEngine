#include <FalconEngine/Input/MouseController.h>

#include <FalconEngine/Context/GameEnginePlatform.h>

#if FALCON_ENGINE_PLATFORM_GLFW
#include <FalconEngine/Input/Platform/GLFW/GLFWMouseControllerData.h>

namespace FalconEngine
{

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
MouseController::MouseController() :
    mData(nullptr)
{
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
void
MouseController::Initialize(MouseControllerDataSharedPtr data)
{
    mData = data;
}

}

#endif
#pragma once

#include <FalconEngine/InputInclude.h>
#include <FalconEngine/Input/MouseState.h>

namespace FalconEngine
{

class Mouse sealed
{
public:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    Mouse();
    ~Mouse();

private:
    MouseState mState;
};

}

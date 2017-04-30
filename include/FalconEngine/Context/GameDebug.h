#pragma once

#include <string>

#include <FalconEngine/Core/Debug.h>

namespace FalconEngine
{

// @summary Debug utility used for both the engine and the game.
class GameDebug
{
public:
    static void
    Break()
    {
        Debug::Break();
    }

    static void
    Initialize();

    static void
    OutputString(const char *str)
    {
        Debug::OutputString(str);
    }

    static void
    OutputString(const std::string& str)
    {
        Debug::OutputString(str);
    }
};

}

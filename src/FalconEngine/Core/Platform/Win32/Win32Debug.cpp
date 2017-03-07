#include <FalconEngine/Core/Debug.h>
#include <FalconEngine/CoreInclude.h>

#if FALCON_ENGINE_OS_WINDOWS

#include <windows.h>
#include <string>

namespace FalconEngine
{

void
Debug::Break()
{
#ifdef _MSC_VER
    // NOTE(Wuxiang): Breakpoints that should ALWAYS trigger (even in release builds)!
    if (IsDebuggerPresent())
    {
        __debugbreak();
    }
#endif
}

/* static */ void
Debug::OutputString(const std::string& str)
{
    OutputDebugString(str.c_str());
}

/* static */ void
Debug::OutputString(char *str)
{
    OutputDebugString(str);
}

}

#endif
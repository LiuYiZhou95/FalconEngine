#pragma once

#include <FalconEngine/Core/Macro.h>

// DLL
#if defined(FALCON_ENGINE_OS_WINDOWS)
#if defined(FALCON_ENGINE_COMPILER_MSVC)
#undef FALCON_ENGINE_ITEM_CONTEXT
#if defined(FALCON_ENGINE_SYMBOL_CONTEXT_EXPORT)
#define FALCON_ENGINE_ITEM_CONTEXT __declspec(dllexport)
#elif defined(FALCON_ENGINE_SYMBOL_CONTEXT_IMPORT)
#define FALCON_ENGINE_ITEM_CONTEXT __declspec(dllimport)
#else
#define FALCON_ENGINE_ITEM_CONTEXT
#endif
#endif
#elif defined(FALCON_ENGINE_OS_LINUX)
#if defined(FALCON_ENGINE_COMPILER_GCC)
#if defined(FALCON_ENGINE_SYMBOL_CONTEXT_EXPORT)
#define FALCON_ENGINE_ITEM_CONTEXT __attribute__ ((visibility("default")))
#else
#define FALCON_ENGINE_ITEM_CONTEXT
#endif
#endif
#endif


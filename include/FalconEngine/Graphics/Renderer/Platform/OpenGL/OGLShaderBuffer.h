#pragma once

#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLMapping.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLBuffer.h>
#include <FalconEngine/Graphics/Renderer/Resource/ShaderBuffer.h>

namespace FalconEngine
{

class FALCON_ENGINE_API PlatformShaderBuffer : public PlatformBuffer
{
public:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    explicit PlatformShaderBuffer(const ShaderBuffer *StorageBuffer);
    ~PlatformShaderBuffer();

public:
    /************************************************************************/
    /* Public Members                                                       */
    /************************************************************************/
    void
    Enable();

    void
    Disable();
};

}

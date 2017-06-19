#pragma once

#include <FalconEngine/Content/Header.h>

#include <FalconEngine/Graphics/Renderer/Resource/Buffer.h>

namespace FalconEngine
{

class FALCON_ENGINE_API TextureImportOption
{
public:
    /************************************************************************/
    /* Static Members                                                       */
    /************************************************************************/
    static const TextureImportOption Default;

public:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    TextureImportOption() = default;

public:
    BufferUsage mTextureUsage = BufferUsage::Static;
};

}
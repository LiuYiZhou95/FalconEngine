#pragma once

#include <memory>
#include <boost/serialization/access.hpp>

#include <FalconEngine/Graphics/Header.h>
#include <FalconEngine/Graphics/Renderer/Resource/Texture.h>

namespace FalconEngine
{

class FALCON_ENGINE_API Texture1d : public Texture
{
public:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    Texture1d(AssetSource assetSource, const std::string& fileName, const std::string& filePath, TextureFormat format, BufferUsage usage = BufferUsage::Dynamic, int mipmapLevel = 0);
    virtual ~Texture1d();
};

}

#include <FalconEngine/Graphics/Renderer/Resource/Texture1d.h>

namespace FalconEngine
{

FALCON_ENGINE_TEXTURE_IMPLEMENT(Texture1d, TextureType::Texture1d);

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
Texture1d::Texture1d()
{
}

Texture1d::Texture1d(AssetSource        assetSource,
                     const std::string& fileName,
                     const std::string& filePath,
                     int                width,
                     TextureFormat      format,
                     BufferUsage        usage,
                     int                mipmapLevel) :
    Texture(assetSource, fileName, filePath, width, 1, 1, format, TextureType::Texture1d, BufferStorageMode::Host, usage, mipmapLevel)
{
}

Texture1d::~Texture1d()
{
}

}

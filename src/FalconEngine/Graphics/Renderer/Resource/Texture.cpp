#include <FalconEngine/Graphics/Renderer/Resource/Texture.h>

#pragma warning(disable : 4244)

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#pragma warning(default : 4244)

namespace FalconEngine
{

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
Texture::Texture() :
    mFormat(TextureFormat::None),
    mMipmapLevel(0),
    mType(TextureType::None),
    mData(nullptr),
    mDataSize(0),
    mStorageMode(BufferStorageMode::Host),
    mUsage(BufferUsage::None)
{
}

Texture::Texture(AssetSource        assetSource,
                 const std::string& fileName,
                 const std::string& filePath,
                 int                width,
                 int                height,
                 int                depth,
                 TextureFormat      format,
                 TextureType        type,
                 BufferUsage        usage,
                 int                mipmapLevel) :
    Asset(assetSource, AssetType::Texture, fileName, filePath),
    mFormat(format),
    mMipmapLevel(mipmapLevel),
    mType(type),
    mStorageMode(BufferStorageMode::Host),
    mUsage(usage)
{
    if (width < 1 || height < 1 || depth < 1)
    {
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Invalid texture dimension.");
    }

    mDimension[0] = width;
    mDimension[1] = height;
    mDimension[2] = depth;

    if (mStorageMode == BufferStorageMode::Host)
    {
        mDataSize = size_t(mDimension[0]) * size_t(mDimension[1]) * size_t(mDimension[2]) * TexelSize[int(mFormat)];
        mData = new unsigned char[mDataSize];
    }
    else
    {
        mData = nullptr;
        mDataSize = 0;
    }
}

Texture::~Texture()
{
    if (mStorageMode == BufferStorageMode::Host)
    {
        delete[] mData;
    }
}

}

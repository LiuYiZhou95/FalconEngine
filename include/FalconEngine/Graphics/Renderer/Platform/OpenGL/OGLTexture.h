#pragma once

#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLMapping.h>

namespace FalconEngine
{

#pragma warning(disable: 4251)
class FALCON_ENGINE_API PlatformTexture
{
public:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    explicit PlatformTexture(const Texture *texture);
    virtual ~PlatformTexture();

public:
    /************************************************************************/
    /* Public Members                                                       */
    /************************************************************************/
    void
    Enable(int textureUnit);

    void
    Disable(int textureUnit);

    void *
    Map(BufferAccessMode          access,
        BufferFlushMode           flush,
        BufferSynchronizationMode synchronization,
        int64_t                   offset,
        int64_t                   size);

    void
    Unmap();

protected:
    GLuint         mBufferObj;

    GLuint         mTextureObj;
    GLuint         mTextureObjPrevious;
    const Texture *mTexturePtr;

    GLuint         mDimension[3];
    GLuint         mFormat;
    GLuint         mFormatInternal;
    GLuint         mType;
    GLuint         mUsage;
};
#pragma warning(default: 4251)

}

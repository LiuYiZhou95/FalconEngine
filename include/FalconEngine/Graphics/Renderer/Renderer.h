#pragma once

#include <FalconEngine/Graphics/Common.h>

#include <map>
#include <unordered_map>
#include <vector>

#include <FalconEngine/Graphics/Renderer/Viewport.h>
#include <FalconEngine/Graphics/Renderer/Window.h>

namespace FalconEngine
{

/************************************************************************/
/* Engine Resource                                                      */
/************************************************************************/
class GameEngineData;
class GameEngineSettings;

/************************************************************************/
/* Rendering Pipeline                                                   */
/************************************************************************/
class Camera;
class Font;
class FontText;
class Primitive;
class Shader;
class ShaderUniform;
class Visual;
class VisualEffectInstance;
class VisualEffectInstancePass;
class VisualEffectPass;

/************************************************************************/
/* Renderer Resource                                                    */
/************************************************************************/
class Buffer;
enum class BufferAccessMode;
enum class BufferFlushMode;
enum class BufferSynchronizationMode;
class IndexBuffer;
class ShaderBuffer;
class VertexBuffer;
class VertexFormat;
class VertexGroup;

class Texture;
class Texture1d;
class Texture2d;
class Texture2dArray;
class Texture3d;
class Sampler;

/************************************************************************/
/* Renderer States                                                      */
/************************************************************************/
class BlendState;
class OffsetState;
class BlendState;
class CullState;
class DepthTestState;
class OffsetState;
class StencilTestState;
class WireframeState;

/************************************************************************/
/* Platform Renderer Resource                                           */
/************************************************************************/
class PlatformShaderBuffer;
class PlatformIndexBuffer;
class PlatformVertexBuffer;
class PlatformVertexFormat;
class PlatformTexture1d;
class PlatformTexture2d;
class PlatformTexture2dArray;
class PlatformTexture3d;
class PlatformSampler;

/************************************************************************/
/* Platform Rendering Pipeline                                          */
/************************************************************************/
class PlatformShader;
class PlatformRendererData;
class PlatformRendererDataDeleter
{
public:
    void operator()(PlatformRendererData *rendererData);
};

#pragma warning(disable: 4251)
class FALCON_ENGINE_API Renderer final
{
public:
    /************************************************************************/
    /* Platform Independent Members                                         */
    /************************************************************************/
    static Renderer *
    GetInstance()
    {
        static Renderer sInstance;
        return &sInstance;
    }

    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
private:
    Renderer();

public:
    ~Renderer();

    /************************************************************************/
    /* Initialization and Destroy                                           */
    /************************************************************************/
public:
    void
    Initialize();

private:
    void
    InitializeData();

    void
    DestroyData();

public:
    /************************************************************************/
    /* Framebuffer Management                                               */
    /************************************************************************/
    void
    ClearColorBuffer(const Vector4f& color);

    void
    ClearDepthBuffer(float depth);

    void
    ClearStencilBuffer(unsigned int stencil);

    void
    ClearFrameBuffer(const Vector4f& color, float depth, unsigned int stencil);

    /************************************************************************/
    /* Viewport Management                                                  */
    /************************************************************************/
    const Viewport *
    GetViewport() const;

    void
    SetViewport(float x, float y, float width, float height);

private:
    // @note The viewport is specified in right-handed screen
    // coordinates. The origin is the lower-left corner of the screen, the
    // y-axis points upward, and the x-axis points rightward.
    void
    SetViewportData(float x, float y, float width, float height);

public:
    const Window *
    GetWindow() const;

    void
    SetWindow(int width, int height, float near, float far);

private:
    void
    SetWindowData(int width, int height, float near, float far);

public:
    /************************************************************************/
    /* Universal Buffer Management                                          */
    /************************************************************************/
    void
    Bind(const Buffer *buffer);

    void
    Unbind(const Buffer *buffer);

    void
    Enable(const Buffer *buffer);

    void
    Disable(const Buffer *buffer);

    void *
    Map(const Buffer             *buffer,
        BufferAccessMode          access,
        BufferFlushMode           flush,
        BufferSynchronizationMode synchronization,
        int64_t                   offset,
        int64_t                   size);

    void
    Unmap(const Buffer *buffer);

    void
    Flush(const Buffer *buffer,
          int64_t       offset,
          int64_t       size);

    void
    Update(const Buffer             *buffer,
           BufferAccessMode          access,
           BufferFlushMode           flush,
           BufferSynchronizationMode synchronization);

    /************************************************************************/
    /* Shader Buffer Management                                             */
    /************************************************************************/
    void
    Bind(const ShaderBuffer *shaderBuffer);

    void
    Unbind(const ShaderBuffer *shaderBuffer);

    void
    Enable(const ShaderBuffer *shaderBuffer);

    void
    Disable(const ShaderBuffer *shaderBuffer);

    void *
    Map(const ShaderBuffer       *shaderBuffer,
        BufferAccessMode          access,
        BufferFlushMode           flush,
        BufferSynchronizationMode synchronization,
        int64_t                   offset,
        int64_t                   size);

    void
    Unmap(const ShaderBuffer *shaderBuffer);

    void
    Flush(const ShaderBuffer *shaderBuffer,
          int64_t             offset,
          int64_t             size);

    /************************************************************************/
    /* Index Buffer Management                                              */
    /************************************************************************/
    void
    Bind(const IndexBuffer *indexBuffer);

    void
    Unbind(const IndexBuffer *indexBuffer);

    void
    Enable(const IndexBuffer *indexBuffer);

    void
    Disable(const IndexBuffer *indexBuffer);

    void *
    Map(const IndexBuffer        *indexBuffer,
        BufferAccessMode          access,
        BufferFlushMode           flush,
        BufferSynchronizationMode synchronization,
        int64_t                   offset,
        int64_t                   size);

    void
    Unmap(const IndexBuffer *indexBuffer);

    void
    Flush(const IndexBuffer *indexBuffer,
          int64_t            offset,
          int64_t            size);

    /************************************************************************/
    /* Vertex Buffer Management                                             */
    /************************************************************************/
    void
    Bind(const VertexBuffer *vertexBuffer);

    void
    Unbind(const VertexBuffer *vertexBuffer);

    // @param offset - offset into the first data in byte.
    // @param stride - stride between contiguous data in byte.
    void
    Enable(const VertexBuffer *vertexBuffer,
           unsigned int        bindingIndex,
           int64_t             offset,
           int                 stride);

    void
    Disable(const VertexBuffer *vertexBuffer, unsigned int bindingIndex);

    void *
    Map(const VertexBuffer       *vertexBuffer,
        BufferAccessMode          access,
        BufferFlushMode           flush,
        BufferSynchronizationMode synchronization,
        int64_t                   offset,
        int64_t                   size);

    void
    Unmap(const VertexBuffer *vertexBuffer);

    void
    Flush(const VertexBuffer *vertexBuffer, int64_t offset, int64_t size);

    /************************************************************************/
    /* Vertex Format Management                                             */
    /************************************************************************/
    void
    Bind(const VertexFormat *vertexFormat);

    void
    Unbind(const VertexFormat *vertexFormat);

    void
    Enable(const VertexFormat *vertexFormat);

    void
    Disable(const VertexFormat *vertexFormat);

    /************************************************************************/
    /* Vertex Group Management                                              */
    /************************************************************************/
    void
    Enable(const VertexGroup *vertexGroup);

    void
    Disable(const VertexGroup *vertexGroup);

    /************************************************************************/
    /* Universal Texture Management                                         */
    /************************************************************************/
    void
    Bind(const Texture *texture);

    void
    Unbind(const Texture *texture);

    // @summary Provide a uniform interface for all texture.
    void
    Enable(int textureUnit, const Texture *texture);

    // @summary Provide a uniform interface for all texture.
    void
    Disable(int textureUnit, const Texture *texture);

    // NEW(Wuxiang): Add the ability of reading back texture.
    // NOTE(Wuxiang): But I don't know why I need any function to update texture.

    /************************************************************************/
    /* Texture 1D Management                                                */
    /************************************************************************/
    void
    Bind(const Texture1d *texture);

    void
    Unbind(const Texture1d *texture);

    void
    Enable(int textureUnit, const Texture1d *texture);

    void
    Disable(int textureUnit, const Texture1d *texture);

    void *
    Map(const Texture1d          *texture,
        int                       mipmapLevel,
        BufferAccessMode          access,
        BufferFlushMode           flush,
        BufferSynchronizationMode synchronization,
        int64_t                   offset,
        int64_t                   size);

    void
    Unmap(const Texture1d *texture, int mipmapLevel);

    /************************************************************************/
    /* Texture 2D Management                                                */
    /************************************************************************/
    void
    Bind(const Texture2d *texture);

    void
    Unbind(const Texture2d *texture);

    void
    Enable(int textureUnit, const Texture2d *texture);

    void
    Disable(int textureUnit, const Texture2d *texture);

    void *
    Map(const Texture2d          *texture,
        int                       mipmapLevel,
        BufferAccessMode          access,
        BufferFlushMode           flush,
        BufferSynchronizationMode synchronization,
        int64_t                   offset,
        int64_t                   size);

    void
    Unmap(const Texture2d *texture, int mipmapLevel);

    /************************************************************************/
    /* Texture 2D Array Management                                          */
    /************************************************************************/
    void
    Bind(const Texture2dArray *textureArray);

    void
    Unbind(const Texture2dArray *textureArray);

    void
    Enable(int                   textureUnit,
           const Texture2dArray *textureArray);

    void
    Disable(int                   textureUnit,
            const Texture2dArray *textureArray);

    void *
    Map(const Texture2dArray     *textureArray,
        int                       textureIndex,
        int                       mipmapLevel,
        BufferAccessMode          access,
        BufferFlushMode           flush,
        BufferSynchronizationMode synchronization,
        int64_t                   offset,
        int64_t                   size);

    void
    Unmap(const Texture2dArray *textureArray,
          int                   textureIndex,
          int                   mipmapLevel);

    /************************************************************************/
    /* Texture 3D Management                                                */
    /************************************************************************/
    void
    Bind(const Texture3d *texture);

    void
    Unbind(const Texture3d *texture);

    void
    Enable(int textureUnit, const Texture3d *texture);

    void
    Disable(int textureUnit, const Texture3d *texture);

    /************************************************************************/
    /* Sampler Management                                                   */
    /************************************************************************/
    void
    Bind(const Sampler *sampler);

    void
    Unbind(const Sampler *sampler);

    void
    Enable(int textureUnit, const Sampler *sampler);

    void
    Disable(int textureUnit, const Sampler *sampler);

    /************************************************************************/
    /* Shader Management                                                   */
    /************************************************************************/
    void
    Bind(Shader *shader);

    void
    Unbind(const Shader *shader);

    void
    Enable(Shader *shader);

    void
    Disable(const Shader *shader);

    /************************************************************************/
    /* Pass Management                                                      */
    /************************************************************************/
    void
    Enable(const VisualEffectPass *pass);

    void
    Enable(const VisualEffectInstancePass *pass, const Camera *camera, const Visual *visual);

    // @summary Update effect instance's uniform.
    void
    Update(const VisualEffectInstancePass *pass, ShaderUniform *uniform, const Camera *camera, const Visual *visual);

    /************************************************************************/
    /* Draw                                                                 */
    /************************************************************************/
    // @summary Draw single instance of visual.
    void
    Draw(const Camera *camera, const Visual *visual);

    // @summary Draw single instance of visual.
    void
    Draw(const Camera *camera, const Visual *visual, VisualEffectInstance *visualEffectInstance);

private:
    /************************************************************************/
    /* Platform Resource Table                                              */
    /************************************************************************/
    // NOTE(Wuxiang): The reason I choose to use std::map here is that the element
    // number is relatively small to a point that the performance gain is not
    // significant.
    std::map<const IndexBuffer *, PlatformIndexBuffer *>       mIndexBufferTable;
    std::map<const ShaderBuffer *, PlatformShaderBuffer *>     mShaderBufferTable;
    std::map<const VertexBuffer *, PlatformVertexBuffer *>     mVertexBufferTable;
    std::map<const VertexFormat *, PlatformVertexFormat *>     mVertexFormatTable;

    std::map<const Shader *, PlatformShader *>                 mShaderTable;
    std::map<const Sampler *, PlatformSampler *>               mSamplerTable;
    std::map<const Texture1d *, PlatformTexture1d *>           mTexture1dTable;
    std::map<const Texture2d *, PlatformTexture2d *>           mTexture2dTable;
    std::map<const Texture2dArray *, PlatformTexture2dArray *> mTexture2dArrayTable;

    /************************************************************************/
    /* Dirty Flags                                                          */
    /************************************************************************/
    const IndexBuffer             *mIndexBufferPrevious;
    const ShaderBuffer            *mShaderBufferPrevious;
    const VertexGroup             *mVertexGroupPrevious;
    const VertexFormat            *mVertexFormatPrevious;

    const VisualEffectPass        *mPassPrevious;

    Shader                        *mShaderPrevious;

    // Sampler table indexed by texture binding index.
    std::map<int, const Sampler *> mSamplerPrevious;

    // Texture table indexed by texture binding index.
    std::map<int, const Texture *> mTexturePrevious;

    /************************************************************************/
    /* Renderer State                                                       */
    /************************************************************************/
    std::unique_ptr<BlendState>       mBlendStateDefault;
    std::unique_ptr<CullState>        mCullStateDefault;
    std::unique_ptr<DepthTestState>   mDepthTestStateDefault;
    std::unique_ptr<OffsetState>      mOffsetStateDefault;
    std::unique_ptr<StencilTestState> mStencilTestStateDefault;
    std::unique_ptr<WireframeState>   mWireframeStateDefault;

    const BlendState                 *mBlendStateCurrent;
    const CullState                  *mCullStateCurrent;
    const DepthTestState             *mDepthTestStateCurrent;
    const OffsetState                *mOffsetStateCurrent;
    const StencilTestState           *mStencilTestStateCurrent;
    const WireframeState             *mWireframeStateCurrent;

    Viewport                          mViewport;
    Window                            mWindow;
    bool                              mWindowInitialized = false;
private:
    /************************************************************************/
    /* Platform Dependent Members                                           */
    /************************************************************************/

    /************************************************************************/
    /* Initialization and Destroy                                           */
    /************************************************************************/
    void
    InitializePlatform();

    void
    DestroyPlatform();

    /************************************************************************/
    /* State Management                                                     */
    /************************************************************************/
    void
    SetBlendStatePlatform(const BlendState *blendState);

    void
    SetCullStatePlatform(const CullState *cullState);

    void
    SetDepthTestStatePlatform(const DepthTestState *depthTestState);

    void
    SetOffsetStatePlatform(const OffsetState *offsetState);

    void
    SetStencilTestStatePlatform(const StencilTestState *stencilTestState);

    void
    SetWireframeStatePlatform(const WireframeState *wireframeState);

public:
    /************************************************************************/
    /* Viewport Management                                                  */
    /************************************************************************/
    // @note The viewport is specified in right-handed screen
    // coordinates. The origin is the lower-left corner of the screen, the
    // y-axis points upward, and the x-axis points rightward.
    void
    SetViewportPlatform(float x, float y, float width, float height);

    void
    SetWindowPlatform(int width, int height, float near, float far);

public:
    /************************************************************************/
    /* Framebuffer Management                                               */
    /************************************************************************/
    void
    ClearColorBufferPlatform(const Vector4f& color);

    void
    ClearDepthBufferPlatform(float depth);

    void
    ClearStencilBufferPlatform(unsigned int stencil);

    void
    ClearFrameBufferPlatform(const Vector4f& color, float depth, unsigned int stencil);

    void
    SwapFrameBufferPlatform();

    /************************************************************************/
    /* Draw                                                                 */
    /************************************************************************/
    void
    DrawPrimitivePlatform(const Primitive *primitive, int instancingNum);

private:
    std::unique_ptr<PlatformRendererData, PlatformRendererDataDeleter> mData;
    bool                                                               mDataInitialized = false;
};
#pragma warning(default: 4251)

}

/************************************************************************/
/* Resource Management Helper                                           */
/************************************************************************/
#define FALCON_ENGINE_RENDERER_BIND(resource) \
static auto sMasterRenderer = FalconEngine::Renderer::GetInstance(); \
sMasterRenderer->Bind(resource);

#define FALCON_ENGINE_RENDERER_UNBIND(resource) \
static auto sMasterRenderer = FalconEngine::Renderer::GetInstance(); \
sMasterRenderer->Unbind(resource);

/************************************************************************/
/* Resource Management Implementation                                   */
/************************************************************************/
#define FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(resource, resourceTable, PlatformResourceKlass) \
FALCON_ENGINE_CHECK_NULLPTR(resource); \
\
if (resourceTable.find(resource) == resourceTable.end()) \
{ \
    resourceTable[resource] = new PlatformResourceKlass(resource); \
}

#define FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(resource, resourceTable) \
FALCON_ENGINE_CHECK_NULLPTR(resource); \
\
auto iter = resourceTable.find(resource); \
if (iter != resourceTable.end()) \
{ \
    auto resource##Platform = iter->second; \
    delete resource##Platform; \
    resourceTable.erase(iter); \
}

#define FALCON_ENGINE_RENDERER_ENABLE_LAZY(resource, resourcePrevious) \
FALCON_ENGINE_CHECK_NULLPTR(resource); \
\
if (resource == resourcePrevious) \
{ \
    return; \
} \
else \
{ \
    if (resourcePrevious) \
    { \
        Disable(resourcePrevious); \
    } \
    resourcePrevious = resource; \
}

#define FALCON_ENGINE_RENDERER_ENABLE_IMPLEMENT(resource, resourceTable, PlatformResourceKlass) \
FALCON_ENGINE_CHECK_NULLPTR(resource); \
\
auto iter = resourceTable.find(resource); \
PlatformResourceKlass *resource##Platform; \
if (iter != resourceTable.end()) \
{ \
    resource##Platform = iter->second; \
} \
else \
{ \
    resource##Platform = new PlatformResourceKlass(resource); \
    resourceTable[resource] = resource##Platform; \
} \
\
resource##Platform->Enable();

#define FALCON_ENGINE_RENDERER_DISABLE_IMPLEMENT(resource, resourceTable) \
FALCON_ENGINE_CHECK_NULLPTR(resource); \
\
auto iter = resourceTable.find(resource); \
if (iter != resourceTable.end()) \
{ \
    auto resource##Platform = iter->second; \
    resource##Platform->Disable(); \
}

#define FALCON_ENGINE_RENDERER_MAP_IMPLEMENT(resource, resourceTable, PlatformResourceKlass) \
FALCON_ENGINE_CHECK_NULLPTR(resource); \
\
auto iter = resourceTable.find(resource); \
PlatformResourceKlass *resource##Platform; \
if (iter != resourceTable.end()) \
{ \
    resource##Platform = iter->second; \
} \
else \
{ \
    resource##Platform = new PlatformResourceKlass(resource); \
    resourceTable[resource] = resource##Platform; \
} \
\
return resource##Platform->Map(access, flush, synchronization, offset, size);

#define FALCON_ENGINE_RENDERER_UNMAP_IMPLEMENT(resource, resourceTable) \
FALCON_ENGINE_CHECK_NULLPTR(resource); \
\
auto iter = resourceTable.find(resource); \
if (iter != resourceTable.end()) \
{ \
    auto resource##Platform = iter->second; \
    resource##Platform->Unmap(); \
}

#define FALCON_ENGINE_RENDERER_FLUSH_IMPLEMENT(resource, resourceTable) \
FALCON_ENGINE_CHECK_NULLPTR(resource); \
\
auto iter = resourceTable.find(resource); \
if (iter != resourceTable.end()) \
{ \
    auto resource##Platform = iter->second; \
    resource##Platform->Flush(offset, size); \
} \
else \
{ \
    FALCON_ENGINE_THROW_RUNTIME_EXCEPTION(std::string("The ") + #resource + " is not mapped before."); \
}

#define FALCON_ENGINE_RENDERER_TEXTURE_ENABLE_LAZY(texture, texturePrevious) \
FALCON_ENGINE_CHECK_NULLPTR(texture); \
\
if (texturePrevious[textureUnit] == texture) \
{ \
    return; \
} \
else \
{ \
    if (texturePrevious[textureUnit]) \
    { \
        Disable(textureUnit, texturePrevious[textureUnit]); \
    } \
\
    texturePrevious[textureUnit] = texture; \
}

#define FALCON_ENGINE_RENDERER_TEXTURE_ENABLE_IMPLEMENT(texture, textureTable, PlatformTextureKlass) \
FALCON_ENGINE_CHECK_NULLPTR(texture); \
\
auto iter = textureTable.find(texture); \
PlatformTextureKlass *texture##Platform; \
if (iter != textureTable.end()) \
{ \
    texture##Platform = iter->second; \
} \
else \
{ \
    texture##Platform = new PlatformTextureKlass(texture); \
    textureTable[texture] = texture##Platform; \
} \
\
texture##Platform->Enable(textureUnit);

#define FALCON_ENGINE_RENDERER_TEXTURE_DISABLE_IMPLEMENT(texture, textureTable) \
FALCON_ENGINE_CHECK_NULLPTR(texture); \
\
auto iter = textureTable.find(texture); \
if (iter != textureTable.end()) \
{ \
    auto texture##Platform = iter->second; \
    texture##Platform->Disable(textureUnit); \
}

#define FALCON_ENGINE_RENDERER_TEXTURE_MAP_IMPLEMENT(texture, textureTable, PlatformTextureKlass) \
FALCON_ENGINE_CHECK_NULLPTR(texture); \
\
auto iter = textureTable.find(texture); \
PlatformTextureKlass *texturePlatform; \
if (iter != textureTable.end()) \
{ \
    texturePlatform = iter->second; \
} \
else \
{ \
    texturePlatform = new PlatformTextureKlass(texture); \
    textureTable[texture] = texturePlatform; \
} \
\
return texturePlatform->Map(access, flush, synchronization, offset, size);

#define FALCON_ENGINE_RENDERER_TEXTURE_UNMAP_IMPLEMENT(texture, textureTable) \
FALCON_ENGINE_CHECK_NULLPTR(texture); \
\
auto iter = textureTable.find(texture); \
if (iter != textureTable.end()) \
{ \
    auto texturePlatform = iter->second; \
    texturePlatform->Unmap(); \
}

#define FALCON_ENGINE_RENDERER_TEXTURE_ARRAY_MAP_IMPLEMENT(textureArray, textureArrayTable, PlatformTextureArrayKlass) \
FALCON_ENGINE_CHECK_NULLPTR(textureArray); \
\
auto iter = textureArrayTable.find(textureArray); \
PlatformTextureArrayKlass *texturePlatform; \
if (iter != textureArrayTable.end()) \
{ \
    texturePlatform = iter->second; \
} \
else \
{ \
    texturePlatform = new PlatformTextureArrayKlass(textureArray); \
    textureArrayTable[textureArray] = texturePlatform; \
} \
\
return texturePlatform->Map(textureIndex, access, flush, synchronization, offset, size);

#define FALCON_ENGINE_RENDERER_TEXTURE_ARRAY_UNMAP_IMPLEMENT(textureArray, textureArrayTable) \
FALCON_ENGINE_CHECK_NULLPTR(textureArray); \
\
auto iter = textureArrayTable.find(textureArray); \
if (iter != textureArrayTable.end()) \
{ \
    auto texturePlatform = iter->second; \
    texturePlatform->Unmap(textureIndex); \
}

#include <FalconEngine/Graphics/Renderer/Renderer.h>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace std;

#include <FalconEngine/Context/GameEngineSettings.h>
#include <FalconEngine/Graphics/Renderer/Camera.h>
#include <FalconEngine/Graphics/Renderer/Scene/Visual.h>
#include <FalconEngine/Graphics/Renderer/VisualEffect.h>
#include <FalconEngine/Graphics/Renderer/VisualEffectInstance.h>
#include <FalconEngine/Graphics/Renderer/VisualEffectInstancePass.h>
#include <FalconEngine/Graphics/Renderer/VisualEffectPass.h>
#include <FalconEngine/Graphics/Renderer/Font/Font.h>
#include <FalconEngine/Graphics/Renderer/Font/FontRenderer.h>
#include <FalconEngine/Graphics/Renderer/Font/FontText.h>
#include <FalconEngine/Graphics/Renderer/Shader/Shader.h>
#include <FalconEngine/Graphics/Renderer/State/BlendState.h>
#include <FalconEngine/Graphics/Renderer/State/CullState.h>
#include <FalconEngine/Graphics/Renderer/State/DepthTestState.h>
#include <FalconEngine/Graphics/Renderer/State/OffsetState.h>
#include <FalconEngine/Graphics/Renderer/State/StencilTestState.h>
#include <FalconEngine/Graphics/Renderer/State/WireframeState.h>
#include <FalconEngine/Graphics/Renderer/Resource/IndexBuffer.h>
#include <FalconEngine/Graphics/Renderer/Resource/ShaderBuffer.h>
#include <FalconEngine/Graphics/Renderer/Resource/VertexBuffer.h>
#include <FalconEngine/Graphics/Renderer/Resource/VertexFormat.h>
#include <FalconEngine/Graphics/Renderer/Resource/VertexGroup.h>
#include <FalconEngine/Graphics/Renderer/Resource/Texture1d.h>
#include <FalconEngine/Graphics/Renderer/Resource/Texture2d.h>
#include <FalconEngine/Graphics/Renderer/Resource/Texture2dArray.h>
#include <FalconEngine/Graphics/Renderer/Resource/Texture3d.h>
#include <FalconEngine/Graphics/Renderer/Resource/Sampler.h>

#if defined(FALCON_ENGINE_API_OPENGL)
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLIndexBuffer.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLVertexBuffer.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLVertexFormat.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLTexture1d.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLTexture2d.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLTexture2dArray.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLTexture3d.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLTextureSampler.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLShader.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLShaderBuffer.h>
#include <FalconEngine/Graphics/Renderer/Platform/OpenGL/OGLShaderUniform.h>
#endif

#if defined(FALCON_ENGINE_WINDOW_GLFW)
#include <FalconEngine/Context/Platform/GLFW/GLFWGameEngineData.h>
#include <FalconEngine/Graphics/Renderer/Platform/GLFW/GLFWRendererData.h>
#endif

namespace FalconEngine
{

void
PlatformRendererDataDeleter::operator()(PlatformRendererData *rendererData)
{
    delete rendererData;
}

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
Renderer::Renderer() :
    mIndexBufferPrevious(nullptr),
    mVertexGroupPrevious(nullptr),
    mVertexFormatPrevious(nullptr),
    mPassPrevious(nullptr),
    mShaderPrevious(nullptr),
    mBlendStateCurrent(nullptr),
    mCullStateCurrent(nullptr),
    mDepthTestStateCurrent(nullptr),
    mOffsetStateCurrent(nullptr),
    mStencilTestStateCurrent(nullptr),
    mWireframeStateCurrent(nullptr)
{
}

Renderer::~Renderer()
{
    DestroyData();
    DestroyPlatform();
}

/************************************************************************/
/* Initialization and Destroy                                           */
/************************************************************************/
void
Renderer::Initialize()
{
    InitializeData();
    InitializePlatform();
}

void
Renderer::InitializeData()
{
    auto gameEngineSettings = GameEngineSettings::GetInstance();

    SetWindowData(gameEngineSettings->mWindowWidth,
                  gameEngineSettings->mWindowHeight,
                  gameEngineSettings->mWindowNear,
                  gameEngineSettings->mWindowFar);

    SetViewportData(0.0f, 0.0f,
                    float(gameEngineSettings->mWindowWidth),
                    float(gameEngineSettings->mWindowHeight));

    mBlendStateDefault = make_unique<BlendState>();
    mCullStateDefault = make_unique<CullState>();
    mDepthTestStateDefault = make_unique<DepthTestState>();
    mOffsetStateDefault = make_unique<OffsetState>();
    mStencilTestStateDefault = make_unique<StencilTestState>();
    mWireframeStateDefault = make_unique<WireframeState>();

    mBlendStateCurrent = mBlendStateDefault.get();
    mCullStateCurrent = mCullStateDefault.get();
    mDepthTestStateCurrent = mDepthTestStateDefault.get();
    mOffsetStateCurrent = mOffsetStateDefault.get();
    mStencilTestStateCurrent = mStencilTestStateDefault.get();
    mWireframeStateCurrent = mWireframeStateDefault.get();
}

void
Renderer::DestroyData()
{
}

/************************************************************************/
/* Framebuffer Management                                               */
/************************************************************************/
void
Renderer::ClearColorBuffer(const Vector4f& color)
{
    ClearColorBufferPlatform(color);
}

void
Renderer::ClearDepthBuffer(float depth)
{
    ClearDepthBufferPlatform(depth);
}

void
Renderer::ClearStencilBuffer(unsigned stencil)
{
    ClearStencilBufferPlatform(stencil);
}

void
Renderer::ClearFrameBuffer(const Vector4f& color, float depth, unsigned stencil)
{
    ClearFrameBufferPlatform(color, depth, stencil);
}

/************************************************************************/
/* Viewport Management                                                  */
/************************************************************************/
const Viewport *
Renderer::GetViewport() const
{
    return &mViewport;
}

void
Renderer::SetViewportData(float x, float y, float width, float height)
{
    if (mWindowInitialized)
    {
        mViewport.Set(x, y, x + width, y + height, mWindow.mNear, mWindow.mFar);
    }
    else
    {
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Need to initialize window first.");
    }
}

void
Renderer::SetViewport(float x, float y, float width, float height)
{
    SetViewportData(x, y, width, height);
    SetViewportPlatform(x, y, width, height);
}

const Window *
Renderer::GetWindow() const
{
    return &mWindow;
}

void
Renderer::SetWindow(int width, int height, float near, float far)
{
    SetWindowData(width, height, near, far);
    SetWindowPlatform(width, height, near, far);
}

void
Renderer::SetWindowData(int width, int height, float near, float far)
{
    mWindow.mWidth = width;
    mWindow.mHeight = height;
    mWindow.mNear = near;
    mWindow.mFar = far;

    mWindowInitialized = true;
}

/************************************************************************/
/* Universal Buffer Management                                          */
/************************************************************************/
void
Renderer::Bind(const Buffer *buffer)
{
    switch (buffer->GetType())
    {
    case BufferType::None:
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Cannot operate on an untyped buffer.");
    case BufferType::VertexBuffer:
        Bind(reinterpret_cast<const VertexBuffer *>(buffer));
        break;
    case BufferType::IndexBuffer:
        Bind(reinterpret_cast<const IndexBuffer *>(buffer));
        break;
    case BufferType::ShaderBuffer:
        Bind(reinterpret_cast<const ShaderBuffer *>(buffer));
        break;
    case BufferType::UniformBuffer:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void
Renderer::Unbind(const Buffer *buffer)
{
    switch (buffer->GetType())
    {
    case BufferType::None:
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Cannot operate on an untyped buffer.");
    case BufferType::VertexBuffer:
        Unbind(reinterpret_cast<const VertexBuffer *>(buffer));
        break;
    case BufferType::IndexBuffer:
        Unbind(reinterpret_cast<const IndexBuffer *>(buffer));
        break;
    case BufferType::ShaderBuffer:
        Unbind(reinterpret_cast<const ShaderBuffer *>(buffer));
        break;
    case BufferType::UniformBuffer:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void
Renderer::Enable(const Buffer *buffer)
{
    switch (buffer->GetType())
    {
    case BufferType::None:
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Cannot operate on an untyped buffer.");
    case BufferType::VertexBuffer:
        Enable(reinterpret_cast<const VertexBuffer *>(buffer));
        break;
    case BufferType::IndexBuffer:
        Enable(reinterpret_cast<const IndexBuffer *>(buffer));
        break;
    case BufferType::ShaderBuffer:
        Enable(reinterpret_cast<const ShaderBuffer *>(buffer));
        break;
    case BufferType::UniformBuffer:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void
Renderer::Disable(const Buffer *buffer)
{
    switch (buffer->GetType())
    {
    case BufferType::None:
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Cannot operate on an untyped buffer.");
    case BufferType::VertexBuffer:
        Disable(reinterpret_cast<const VertexBuffer *>(buffer));
        break;
    case BufferType::IndexBuffer:
        Disable(reinterpret_cast<const IndexBuffer *>(buffer));
        break;
    case BufferType::ShaderBuffer:
        Disable(reinterpret_cast<const ShaderBuffer *>(buffer));
        break;
    case BufferType::UniformBuffer:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void *
Renderer::Map(const Buffer *buffer, BufferAccessMode access, BufferFlushMode flush, BufferSynchronizationMode synchronization, int64_t offset, int64_t size)
{
    switch (buffer->GetType())
    {
    case BufferType::None:
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Cannot operate on an untyped buffer.");
    case BufferType::VertexBuffer:
        return Map(reinterpret_cast<const VertexBuffer *>(buffer), access, flush, synchronization, offset, size);
    case BufferType::IndexBuffer:
        return Map(reinterpret_cast<const IndexBuffer *>(buffer), access, flush, synchronization, offset, size);
    case BufferType::ShaderBuffer:
        return Map(reinterpret_cast<const ShaderBuffer *>(buffer), access, flush, synchronization, offset, size);
    case BufferType::UniformBuffer:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void
Renderer::Unmap(const Buffer *buffer)
{
    switch (buffer->GetType())
    {
    case BufferType::None:
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Cannot operate on an untyped buffer.");
    case BufferType::VertexBuffer:
        Unmap(reinterpret_cast<const VertexBuffer *>(buffer));
        break;
    case BufferType::IndexBuffer:
        Unmap(reinterpret_cast<const IndexBuffer *>(buffer));
        break;
    case BufferType::ShaderBuffer:
        Unmap(reinterpret_cast<const ShaderBuffer *>(buffer));
        break;
    case BufferType::UniformBuffer:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void
Renderer::Flush(const Buffer *buffer, int64_t offset, int64_t size)
{
    switch (buffer->GetType())
    {
    case BufferType::None:
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Cannot operate on an untyped buffer.");
    case BufferType::VertexBuffer:
        Flush(reinterpret_cast<const VertexBuffer *>(buffer), offset, size);
        break;
    case BufferType::IndexBuffer:
        Flush(reinterpret_cast<const IndexBuffer *>(buffer), offset, size);
        break;
    case BufferType::ShaderBuffer:
        Flush(reinterpret_cast<const ShaderBuffer *>(buffer), offset, size);
        break;
    case BufferType::UniformBuffer:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void
Renderer::Update(const Buffer             *buffer,
                 BufferAccessMode          access,
                 BufferFlushMode           flush,
                 BufferSynchronizationMode synchronization)
{
    auto sourceData = buffer->GetData() + buffer->GetDataOffset();

    auto destinationData = Map(buffer, access, flush, synchronization,
                               buffer->GetDataOffset(), buffer->GetDataSize());

    memcpy(destinationData, sourceData, buffer->GetDataSize());

    Unmap(buffer);
}

/************************************************************************/
/* Shader Buffer Management                                             */
/************************************************************************/
void
Renderer::Bind(const ShaderBuffer *shaderBuffer)
{
    FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(shaderBuffer, mShaderBufferTable, PlatformShaderBuffer);
}

void
Renderer::Unbind(const ShaderBuffer *shaderBuffer)
{
    FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(shaderBuffer, mShaderBufferTable);
}

void
Renderer::Enable(const ShaderBuffer *shaderBuffer)
{
    FALCON_ENGINE_RENDERER_ENABLE_LAZY(shaderBuffer, mShaderBufferPrevious);
    FALCON_ENGINE_RENDERER_ENABLE_IMPLEMENT(shaderBuffer, mShaderBufferTable, PlatformShaderBuffer);
}

void
Renderer::Disable(const ShaderBuffer *shaderBuffer)
{
    FALCON_ENGINE_RENDERER_DISABLE_IMPLEMENT(shaderBuffer, mShaderBufferTable);
}

void *
Renderer::Map(const ShaderBuffer *shaderBuffer, BufferAccessMode access, BufferFlushMode flush, BufferSynchronizationMode synchronization, int64_t offset, int64_t size)
{
    FALCON_ENGINE_RENDERER_MAP_IMPLEMENT(shaderBuffer, mShaderBufferTable, PlatformShaderBuffer);
}

void
Renderer::Unmap(const ShaderBuffer *shaderBuffer)
{
    FALCON_ENGINE_RENDERER_UNMAP_IMPLEMENT(shaderBuffer, mShaderBufferTable);
}

void
Renderer::Flush(const ShaderBuffer *shaderBuffer, int64_t offset, int64_t size)
{
    FALCON_ENGINE_RENDERER_FLUSH_IMPLEMENT(shaderBuffer, mShaderBufferTable);
}

/************************************************************************/
/* Index Buffer Management                                              */
/************************************************************************/
void
Renderer::Bind(const IndexBuffer *indexBuffer)
{
    FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(indexBuffer, mIndexBufferTable, PlatformIndexBuffer);
}

void
Renderer::Unbind(const IndexBuffer *indexBuffer)
{
    FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(indexBuffer, mIndexBufferTable);
}

void
Renderer::Enable(const IndexBuffer *indexBuffer)
{
    FALCON_ENGINE_RENDERER_ENABLE_LAZY(indexBuffer, mIndexBufferPrevious);
    FALCON_ENGINE_RENDERER_ENABLE_IMPLEMENT(indexBuffer, mIndexBufferTable, PlatformIndexBuffer);
}

void
Renderer::Disable(const IndexBuffer *indexBuffer)
{
    FALCON_ENGINE_RENDERER_DISABLE_IMPLEMENT(indexBuffer, mIndexBufferTable);
}

void *
Renderer::Map(const IndexBuffer        *indexBuffer,
              BufferAccessMode          access,
              BufferFlushMode           flush,
              BufferSynchronizationMode synchronization,
              int64_t                   offset,
              int64_t                   size)
{
    FALCON_ENGINE_RENDERER_MAP_IMPLEMENT(indexBuffer, mIndexBufferTable, PlatformIndexBuffer);
}

void
Renderer::Unmap(const IndexBuffer *indexBuffer)
{
    FALCON_ENGINE_RENDERER_UNMAP_IMPLEMENT(indexBuffer, mIndexBufferTable);
}

void
Renderer::Flush(const IndexBuffer *indexBuffer, int64_t offset, int64_t size)
{
    FALCON_ENGINE_RENDERER_FLUSH_IMPLEMENT(indexBuffer, mIndexBufferTable);
}

/************************************************************************/
/* Vertex Buffer Management                                             */
/************************************************************************/
void
Renderer::Bind(const VertexBuffer *vertexBuffer)
{
    FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(vertexBuffer, mVertexBufferTable, PlatformVertexBuffer);
}

void
Renderer::Unbind(const VertexBuffer *vertexBuffer)
{
    FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(vertexBuffer, mVertexBufferTable);
}

void
Renderer::Enable(const VertexBuffer *vertexBuffer,
                 unsigned int        bindingIndex,
                 int64_t             offset,
                 int                 stride)
{
    FALCON_ENGINE_CHECK_NULLPTR(vertexBuffer);

    auto iter = mVertexBufferTable.find(vertexBuffer);
    PlatformVertexBuffer *vertexBufferPlatform;
    if (iter != mVertexBufferTable.end())
    {
        vertexBufferPlatform = iter->second;
    }
    else
    {
        vertexBufferPlatform = new PlatformVertexBuffer(vertexBuffer);
        mVertexBufferTable[vertexBuffer] = vertexBufferPlatform;
    }

    vertexBufferPlatform->Enable(bindingIndex, offset, stride);
}

void
Renderer::Disable(const VertexBuffer *vertexBuffer, unsigned int bindingIndex)
{
    FALCON_ENGINE_CHECK_NULLPTR(vertexBuffer);

    auto iter = mVertexBufferTable.find(vertexBuffer);
    if (iter != mVertexBufferTable.end())
    {
        auto vertexBufferPlatform = iter->second;
        vertexBufferPlatform->Disable(bindingIndex);
    }
}

void *
Renderer::Map(const VertexBuffer       *vertexBuffer,
              BufferAccessMode          access,
              BufferFlushMode           flush,
              BufferSynchronizationMode synchronization,
              int64_t                   offset,
              int64_t                   size)
{
    FALCON_ENGINE_RENDERER_MAP_IMPLEMENT(vertexBuffer, mVertexBufferTable, PlatformVertexBuffer);
}

void
Renderer::Unmap(const VertexBuffer *vertexBuffer)
{
    FALCON_ENGINE_RENDERER_UNMAP_IMPLEMENT(vertexBuffer, mVertexBufferTable);
}

void
Renderer::Flush(const VertexBuffer *vertexBuffer,
                int64_t             offset,
                int64_t             size)
{
    FALCON_ENGINE_RENDERER_FLUSH_IMPLEMENT(vertexBuffer, mVertexBufferTable);
}

/************************************************************************/
/* Vertex Format Management                                             */
/************************************************************************/
void
Renderer::Bind(const VertexFormat *vertexFormat)
{
    FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(vertexFormat, mVertexFormatTable, PlatformVertexFormat);
}

void
Renderer::Unbind(const VertexFormat *vertexFormat)
{
    FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(vertexFormat, mVertexFormatTable);
}

void
Renderer::Enable(const VertexFormat *vertexFormat)
{
    FALCON_ENGINE_RENDERER_ENABLE_LAZY(vertexFormat, mVertexFormatPrevious);
    FALCON_ENGINE_RENDERER_ENABLE_IMPLEMENT(vertexFormat, mVertexFormatTable, PlatformVertexFormat);
}

void
Renderer::Disable(const VertexFormat *vertexFormat)
{
    FALCON_ENGINE_RENDERER_DISABLE_IMPLEMENT(vertexFormat, mVertexFormatTable);
}

/************************************************************************/
/* Vertex Group Management                                              */
/************************************************************************/
void
Renderer::Enable(const VertexGroup *vertexGroup)
{
    FALCON_ENGINE_RENDERER_ENABLE_LAZY(vertexGroup, mVertexGroupPrevious);

    for (auto vertexBufferBindingIter = vertexGroup->GetVertexBufferBindingBegin();
            vertexBufferBindingIter != vertexGroup->GetVertexBufferBindingEnd();
            ++vertexBufferBindingIter)
    {
        const auto& vertexBufferBinding =
            static_pointer_cast<const VertexBufferBinding>(vertexBufferBindingIter->second);

        Enable(vertexBufferBinding->GetBuffer(),
               vertexBufferBinding->GetIndex(),
               vertexBufferBinding->GetOffset(),
               vertexBufferBinding->GetStride());
    }
}

void
Renderer::Disable(const VertexGroup *vertexGroup)
{
    for (auto vertexBufferBindingIter = vertexGroup->GetVertexBufferBindingBegin();
            vertexBufferBindingIter != vertexGroup->GetVertexBufferBindingEnd();
            ++vertexBufferBindingIter)
    {
        const auto& vertexBufferBinding =
            static_pointer_cast<const VertexBufferBinding>(vertexBufferBindingIter->second);

        Disable(vertexBufferBinding->GetBuffer(),
                vertexBufferBinding->GetIndex());
    }
}

/************************************************************************/
/* Universal Texture Management                                         */
/************************************************************************/
void
Renderer::Bind(const Texture *texture)
{
    switch (texture->mType)
    {
    case TextureType::None:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    case TextureType::Texture1d:
        Bind(reinterpret_cast<const Texture1d *>(texture));
        break;
    case TextureType::Texture2d:
        Bind(reinterpret_cast<const Texture2d *>(texture));
        break;
    case TextureType::Texture2dArray:
        Bind(reinterpret_cast<const Texture2dArray *>(texture));
        break;
    case TextureType::Texture3d:
        Bind(reinterpret_cast<const Texture3d *>(texture));
        break;
    case TextureType::TextureCube:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void
Renderer::Unbind(const Texture *texture)
{
    switch (texture->mType)
    {
    case TextureType::None:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    case TextureType::Texture1d:
        Unbind(reinterpret_cast<const Texture1d *>(texture));
        break;
    case TextureType::Texture2d:
        Unbind(reinterpret_cast<const Texture2d *>(texture));
        break;
    case TextureType::Texture2dArray:
        Unbind(reinterpret_cast<const Texture2dArray *>(texture));
        break;
    case TextureType::Texture3d:
        Unbind(reinterpret_cast<const Texture3d *>(texture));
        break;
    case TextureType::TextureCube:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void
Renderer::Enable(int textureUnit, const Texture *texture)
{
    FALCON_ENGINE_RENDERER_TEXTURE_ENABLE_LAZY(texture, mTexturePrevious);

    switch (texture->mType)
    {
    case TextureType::None:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    case TextureType::Texture1d:
        Enable(textureUnit, reinterpret_cast<const Texture1d *>(texture));
        break;
    case TextureType::Texture2d:
        Enable(textureUnit, reinterpret_cast<const Texture2d *>(texture));
        break;
    case TextureType::Texture2dArray:
        Enable(textureUnit, reinterpret_cast<const Texture2dArray *>(texture));
        break;
    case TextureType::Texture3d:
        Enable(textureUnit, reinterpret_cast<const Texture3d *>(texture));
        break;
    case TextureType::TextureCube:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

void
Renderer::Disable(int textureUnit, const Texture *texture)
{
    FALCON_ENGINE_CHECK_NULLPTR(texture);

    switch (texture->mType)
    {
    case TextureType::None:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    case TextureType::Texture1d:
        Disable(textureUnit, reinterpret_cast<const Texture1d *>(texture));
        break;
    case TextureType::Texture2d:
        Disable(textureUnit, reinterpret_cast<const Texture2d *>(texture));
        break;
    case TextureType::Texture2dArray:
        Disable(textureUnit, reinterpret_cast<const Texture2dArray *>(texture));
        break;
    case TextureType::Texture3d:
        Disable(textureUnit, reinterpret_cast<const Texture3d *>(texture));
        break;
    case TextureType::TextureCube:
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    default:
        FALCON_ENGINE_THROW_ASSERTION_EXCEPTION();
    }
}

/************************************************************************/
/* Texture 1D Management                                                */
/************************************************************************/
void
Renderer::Bind(const Texture1d *texture)
{
    FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(texture, mTexture1dTable, PlatformTexture1d);
}

void
Renderer::Unbind(const Texture1d *texture)
{
    FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(texture, mTexture1dTable);
}

void
Renderer::Enable(int textureUnit, const Texture1d *texture)
{
    FALCON_ENGINE_RENDERER_TEXTURE_ENABLE_IMPLEMENT(texture, mTexture1dTable, PlatformTexture1d);
}

void
Renderer::Disable(int textureUnit, const Texture1d *texture)
{
    FALCON_ENGINE_RENDERER_TEXTURE_DISABLE_IMPLEMENT(texture, mTexture1dTable);
}

void *
Renderer::Map(const Texture1d          *texture,
              int                    /* mipmapLevel */,
              BufferAccessMode          access,
              BufferFlushMode           flush,
              BufferSynchronizationMode synchronization,
              int64_t                   offset,
              int64_t                   size)
{
    FALCON_ENGINE_RENDERER_TEXTURE_MAP_IMPLEMENT(texture, mTexture1dTable, PlatformTexture1d);
}

void
Renderer::Unmap(const Texture1d *texture, int /* mipmapLevel */)
{
    FALCON_ENGINE_RENDERER_TEXTURE_UNMAP_IMPLEMENT(texture, mTexture1dTable);
}

/************************************************************************/
/* Texture 2D Management                                                */
/************************************************************************/
void
Renderer::Bind(const Texture2d *texture)
{
    FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(texture, mTexture2dTable, PlatformTexture2d);
}

void
Renderer::Unbind(const Texture2d *texture)
{
    FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(texture, mTexture2dTable);
}

void
Renderer::Enable(int textureUnit, const Texture2d *texture)
{
    FALCON_ENGINE_RENDERER_TEXTURE_ENABLE_IMPLEMENT(texture, mTexture2dTable, PlatformTexture2d);
}

void
Renderer::Disable(int textureUnit, const Texture2d *texture)
{
    FALCON_ENGINE_RENDERER_TEXTURE_DISABLE_IMPLEMENT(texture, mTexture2dTable);
}

void *
Renderer::Map(const Texture2d          *texture,
              int                    /* mipmapLevel */,
              BufferAccessMode          access,
              BufferFlushMode           flush,
              BufferSynchronizationMode synchronization,
              int64_t                   offset,
              int64_t                   size)
{
    FALCON_ENGINE_RENDERER_TEXTURE_MAP_IMPLEMENT(texture, mTexture2dTable, PlatformTexture2d);
}

void
Renderer::Unmap(const Texture2d *texture, int /* mipmapLevel */)
{
    FALCON_ENGINE_RENDERER_TEXTURE_UNMAP_IMPLEMENT(texture, mTexture2dTable);
}

/************************************************************************/
/* Texture 2D Array Management                                          */
/************************************************************************/
void
Renderer::Bind(const Texture2dArray *textureArray)
{
    FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(textureArray, mTexture2dArrayTable, PlatformTexture2dArray);
}

void
Renderer::Unbind(const Texture2dArray *textureArray)
{
    FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(textureArray, mTexture2dArrayTable);
}

void
Renderer::Enable(int textureUnit, const Texture2dArray *textureArray)
{
    FALCON_ENGINE_RENDERER_TEXTURE_ENABLE_IMPLEMENT(textureArray, mTexture2dArrayTable, PlatformTexture2dArray);
}

void
Renderer::Disable(int textureUnit, const Texture2dArray *textureArray)
{
    FALCON_ENGINE_RENDERER_TEXTURE_DISABLE_IMPLEMENT(textureArray, mTexture2dArrayTable);
}

void *
Renderer::Map(const Texture2dArray     *textureArray,
              int                       textureIndex,
              int                    /* mipmapLevel */,
              BufferAccessMode          access,
              BufferFlushMode           flush,
              BufferSynchronizationMode synchronization,
              int64_t                   offset,
              int64_t                   size)
{
    FALCON_ENGINE_RENDERER_TEXTURE_ARRAY_MAP_IMPLEMENT(textureArray, mTexture2dArrayTable, PlatformTexture2dArray);
}

void
Renderer::Unmap(const Texture2dArray *textureArray,
                int                   textureIndex,
                int                /* mipmapLevel */)
{
    FALCON_ENGINE_RENDERER_TEXTURE_ARRAY_UNMAP_IMPLEMENT(textureArray, mTexture2dArrayTable);
}

/************************************************************************/
/* Texture 3D Management                                                */
/************************************************************************/
void
Renderer::Bind(const Texture3d * /* texture */)
{
    FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
}

void
Renderer::Unbind(const Texture3d * /* texture */)
{
    FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
}

void
Renderer::Enable(int /* textureUnit */, const Texture3d * /* texture */)
{
    FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
}

void
Renderer::Disable(int /* textureUnit */, const Texture3d * /* texture */)
{
    FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
}

/************************************************************************/
/* Sampler Management                                                   */
/************************************************************************/
void
Renderer::Bind(const Sampler *sampler)
{
    FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(sampler, mSamplerTable, PlatformSampler);
}

void
Renderer::Unbind(const Sampler *sampler)
{
    FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(sampler, mSamplerTable);
}

void
Renderer::Enable(int textureUnit, const Sampler *sampler)
{
    FALCON_ENGINE_RENDERER_TEXTURE_ENABLE_LAZY(sampler, mSamplerPrevious);
    FALCON_ENGINE_RENDERER_TEXTURE_ENABLE_IMPLEMENT(sampler, mSamplerTable, PlatformSampler);
}

void
Renderer::Disable(int textureUnit, const Sampler *sampler)
{
    FALCON_ENGINE_RENDERER_TEXTURE_DISABLE_IMPLEMENT(sampler, mSamplerTable);
}

/************************************************************************/
/* Shader Management                                                   */
/************************************************************************/
void
Renderer::Bind(Shader *shader)
{
    FALCON_ENGINE_RENDERER_BIND_IMPLEMENT(shader, mShaderTable, PlatformShader);
}

void
Renderer::Unbind(const Shader *shader)
{
    FALCON_ENGINE_RENDERER_UNBIND_IMPLEMENT(shader, mShaderTable);
}

void
Renderer::Enable(Shader *shader)
{
    FALCON_ENGINE_RENDERER_ENABLE_LAZY(shader, mShaderPrevious);
    FALCON_ENGINE_RENDERER_ENABLE_IMPLEMENT(shader, mShaderTable, PlatformShader);
}

void
Renderer::Disable(const Shader *shader)
{
    FALCON_ENGINE_RENDERER_DISABLE_IMPLEMENT(shader, mShaderTable);
}

/************************************************************************/
/* Pass Management                                                      */
/************************************************************************/
void
Renderer::Enable(const VisualEffectPass *pass)
{
    if (pass == mPassPrevious)
    {
        return;
    }
    else
    {
        mPassPrevious = pass;
    }

    // Set pass' render states.
    SetBlendStatePlatform(pass->GetBlendState());
    SetCullStatePlatform(pass->GetCullState());
    SetDepthTestStatePlatform(pass->GetDepthTestState());
    SetOffsetStatePlatform(pass->GetOffsetState());
    SetStencilTestStatePlatform(pass->GetStencilTestState());
    SetWireframeStatePlatform(pass->GetWireframeState());
}

void
Renderer::Enable(const VisualEffectInstancePass *pass, const Camera *camera, const Visual *visual)
{
    FALCON_ENGINE_CHECK_NULLPTR(pass);

    // Enable required shader.
    Enable(pass->GetShader());

    // Enable required shader textures.
    for (auto textureIter = pass->GetShaderTextureBegin(); textureIter != pass->GetShaderTextureEnd(); ++textureIter)
    {
        Enable(textureIter->first, textureIter->second);
    }

    // Enable required shader samplers.
    for (auto samplerIter = pass->GetShaderSamplerBegin(); samplerIter != pass->GetShaderSamplerEnd(); ++samplerIter)
    {
        Enable(samplerIter->first, samplerIter->second);
    }

    // Update required shader uniforms.
    for (int uniformIndex = 0; uniformIndex < pass->GetShaderUniformNum(); ++uniformIndex)
    {
        // NOTE(Wuxiang): At this point you are supposed to have uniform location
        // for each uniform. In principle, you should finish declaring all shader
        // uniform before this. The render engine would not repetitively check
        // whether each uniform has setup uniform location.

        // NOTE(Wuxiang): The location of the shader uniform would be stored in
        // shader's uniform table after the binding of the shader.
        auto uniform = pass->GetShaderUniform(uniformIndex);
        Update(pass, uniform, camera, visual);
    }
}

void
Renderer::Update(const VisualEffectInstancePass *pass, ShaderUniform *uniform, const Camera *camera, const Visual *visual)
{
    // NOTE(Wuxiang): Note that uniform comes from the instance pass. This
    // means there would be user modification.

    // Initialize uniform state and location.
    if (!uniform->mInitialized)
    {
        // NOTE(Wuxiang): This is necessary because the uniform in instance
        // pass is different entity from uniform in shader.
        auto shader = pass->GetShader();
        uniform->mEnabled = shader->IsUniformEnabled(uniform->mName);
        uniform->mLocation = shader->GetUniformLocation(uniform->mName);
        uniform->mInitialized = true;
    }

    // Update uniform value.
    if (uniform->mEnabled)
    {
        if (uniform->IsUpdateNeeded())
        {
            uniform->Update(camera, visual);
        }

        // Update uniform value in context.
        //
        // NOTE(Wuxiang): You have to update a uniform value regardless whether it
        // has been updated or not because the shader program is changed
        // arbitrarily.
        PlatformShaderUniform::UpdateContext(uniform);
    }
}

/************************************************************************/
/* Draw                                                                 */
/************************************************************************/
void
Renderer::Draw(const Camera *camera,
               const Visual *visual)
{
    // NOTE(Wuxiang): We don't need to check the camera is not null here, because
    // certain rendering task won't need camera information.

    FALCON_ENGINE_CHECK_NULLPTR(visual);

    for (auto visualEffectInstanceIter = visual->GetEffectInstanceBegin();
            visualEffectInstanceIter != visual->GetEffectInstanceEnd();
            ++visualEffectInstanceIter)
    {
        // NOTE(Wuxiang): The visual instance is guaranteed to not be null.
        FALCON_ENGINE_CHECK_NULLPTR(*visualEffectInstanceIter);

        Draw(camera, visual, visualEffectInstanceIter->get());
    }
}

void
Renderer::Draw(
    _IN_     const Camera         *camera,
    _IN_     const Visual         *visual,
    _IN_OUT_ VisualEffectInstance *visualEffectInstance)
{
    // NEW(Wuxiang): Add render command sorting.

    // NOTE(Wuxiang): The non-constness of instance comes from the fact that
    // during the binding of shader, the renderer would look up the shader's
    // location for each vertex attribute and each uniform.

    FALCON_ENGINE_CHECK_NULLPTR(visual);
    FALCON_ENGINE_CHECK_NULLPTR(visualEffectInstance);

    // NOTE(Wuxiang): The order of enabling resource depends on the how fast
    // context would switch those resources.

    // NOTE(Wuxiang): Currently this function assume that all passes are using
    // same vertex attribute array, so that we don't switch vertex format between
    // different shader.

    auto visualEffect = visualEffectInstance->GetEffect();

    // Enable vertex attribute array.
    auto vertexFormat = visual->GetVertexFormat();
    Enable(vertexFormat);

    // Enable vertex group and consequently vertex buffer.
    auto vertexGroup = visual->GetVertexGroup();
    Enable(vertexGroup);

    // Fetch primitive in Visual.
    auto primitive = visual->GetMesh()->GetPrimitive();

    auto indexBuffer = primitive->GetIndexBuffer();
    if (indexBuffer)
    {
        // Enable index buffer.
        Enable(indexBuffer);
    }

    const int passNum = visualEffectInstance->GetPassNum();
    for (int passIndex = 0; passIndex < passNum; ++passIndex)
    {
        auto visualEffectInstancePass = visualEffectInstance->GetPass(passIndex);

        // Enable effect pass.
        auto visualEffectPass = visualEffect->GetPass(passIndex);
        Enable(visualEffectPass);

        // Enable effect instance pass.
        Enable(visualEffectInstancePass, camera, visual);

        // Draw primitive.
        auto primitiveInstancingNum = visualEffectInstancePass->GetShaderInstancingNum();
        DrawPrimitivePlatform(primitive, primitiveInstancingNum);
    }
}

}

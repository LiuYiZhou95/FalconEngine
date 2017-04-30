#pragma once

#include <map>

#include <FalconEngine/GraphicsInclude.h>

#if FALCON_ENGINE_PLATFORM_GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#endif

#include <FalconEngine/Graphics/Renderer/Primitive.h>
#include <FalconEngine/Graphics/Renderer/Resource/Buffer.h>
#include <FalconEngine/Graphics/Renderer/Resource/Sampler.h>
#include <FalconEngine/Graphics/Renderer/Resource/Texture.h>
#include <FalconEngine/Graphics/Renderer/Resource/VertexAttribute.h>
#include <FalconEngine/Graphics/Renderer/Shader/Shader.h>
#include <FalconEngine/Graphics/Renderer/State/BlendState.h>
#include <FalconEngine/Graphics/Renderer/State/DepthTestState.h>
#include <FalconEngine/Graphics/Renderer/State/StencilTestState.h>

namespace FalconEngine
{

extern const GLuint OpenGLBufferAccessMode[int(BufferAccessMode::Count)];
extern const GLuint OpenGLBufferUsage[int(BufferUsage::Count)];

extern const GLuint OpenGLTextureType[int(TextureFormat::Count)];               // Texture element type
extern const GLuint OpenGLTextureFormat[int(TextureFormat::Count)];             // Texture format
extern const GLuint OpenGLTextureInternalFormat[int(TextureFormat::Count)];     // Texture internal format
extern const GLuint OpenGLTextureTarget[int(TextureType::Count)];
extern const GLuint OpenGLTextureTargetBinding[int(TextureType::Count)];

extern const GLint  OpenGLSamplerFilterMode[int(SamplerMinificationFilter::Count)];
extern const GLint  OpenGLSamplerWrapMode[int(SamplerWrapMode::Count)];

extern const GLenum OpenGLBlendFactorSource[int(BlendSourceFactor::Count)];
extern const GLenum OpenGLBlendFactorDestination[int(BlendDestinationFactor::Count)];

extern const GLenum OpenGLDepthFunction[int(DepthFunction::Count)];

extern const GLenum OpenGLStencilFunction[int(StencilFunction::Count)];
extern const GLenum OpenGLStencilOperation[int(StencilOperation::Count)];

extern const GLuint OpenGLShaderAttributeType[int(VertexAttributeType::Count)];
extern const GLenum OpenGLShaderType[int(ShaderType::Count)];

extern const GLenum OpenGLPrimitiveType[int(PrimitiveType::Count)];

// @return previous bound texture
GLuint
BindTexture(TextureType textureType, GLuint texture);

// @return previous bound texture
GLuint
GetBoundTexture(TextureType textureType);

// @return previous bound sampler
GLuint
BindSampler(GLuint textureUnit, GLuint sampler);

}

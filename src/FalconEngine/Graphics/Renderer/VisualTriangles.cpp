#include <FalconEngine/Graphics/Renderer/VisualTriangles.h>
#include <FalconEngine/Graphics/Renderer/Resources/IndexBuffer.h>
#include <FalconEngine/Graphics/Renderer/Resources/VertexGroup.h>

namespace FalconEngine
{

FALCON_ENGINE_RTTI_IMPLEMENT(VisualTriangles, Visual);

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
VisualTriangles::VisualTriangles(VertexFormatSharedPtr vertexFormat) :
    Visual(PrimitiveType::Triangle, vertexFormat)
{
}

VisualTriangles::VisualTriangles(VertexFormatSharedPtr vertexFormat, VertexGroupSharedPtr vertexGroup, IndexBufferSharedPtr indexBuffer) :
    Visual(PrimitiveType::Triangle, vertexFormat, vertexGroup, indexBuffer)
{
}

VisualTriangles::~VisualTriangles()
{
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
size_t
VisualTriangles::GetTriangleNum() const
{
    if (mIndexBuffer)
    {
        return mIndexBuffer->GetElementNum() / 3;
    }
    else
    {
        return mVertexGroup->GetVertexNum() / 3;
    }
}

}

#pragma once

#include <FalconEngine/GraphicsInclude.h>
#include <FalconEngine/Core/Object.h>

namespace FalconEngine
{

enum class PrimitiveType
{
    None,
    Point,
    Line,
    LineStrip,
    Triangle,
    Count,
};

class BoundingBox;
using BoundingBoxSharedPtr = std::shared_ptr<BoundingBox>;

class IndexBuffer;
using IndexBufferSharedPtr = std::shared_ptr<IndexBuffer>;

class VertexBuffer;
using VertexBufferSharedPtr = std::shared_ptr<VertexBuffer>;

class VertexFormat;
using VertexFormatSharedPtr = std::shared_ptr<VertexFormat>;

class VertexGroup;
using VertexGroupSharedPtr = std::shared_ptr<VertexGroup>;

class Primitive : public Object
{
    FALCON_ENGINE_RTTI_DECLARE;

protected:
    Primitive(PrimitiveType primitiveType, VertexFormatSharedPtr vertexFormat);
    Primitive(PrimitiveType primitiveType, VertexFormatSharedPtr vertexFormat, VertexGroupSharedPtr vertexGroup, IndexBufferSharedPtr indexBuffer);

public:
    virtual ~Primitive();

public:
    PrimitiveType
    GetPrimitiveType() const;

    BoundingBoxSharedPtr
    GetBoundingBox() const;

    void
    SetBoundingBox(BoundingBoxSharedPtr boundingBox);

    /************************************************************************/
    /* Vertex Buffer Management                                             */
    /************************************************************************/
    void
    SetVertexBuffer(int bindingIndex, VertexBufferSharedPtr vertexBuffer, int offset, int stride);

    const VertexGroup *
    GetVertexGroup() const;

    size_t
    GetVertexNum() const;

    /************************************************************************/
    /* Vertex Format Management                                             */
    /************************************************************************/
    const VertexFormat *
    GetVertexFormat() const;

    void
    SetVertexFormat(VertexFormatSharedPtr vertexFormat);

    /************************************************************************/
    /* Index Buffer Management                                              */
    /************************************************************************/
    const IndexBuffer *
    GetIndexBuffer() const;

    void
    SetIndexBuffer(IndexBufferSharedPtr indexBuffer);

protected:
    BoundingBoxSharedPtr  mBoundingBox;
    PrimitiveType         mPrimitiveType;
    VertexGroupSharedPtr  mVertexGroup;
    VertexFormatSharedPtr mVertexFormat;
    IndexBufferSharedPtr  mIndexBuffer;
};

}
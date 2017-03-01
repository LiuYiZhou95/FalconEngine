#pragma once

#include <FalconEngine/GraphicsInclude.h>

#include <stdexcept>
#include <memory>

#include <FalconEngine/Graphics/Renderers/Resources/Buffer.h>

namespace FalconEngine
{

enum class IndexType
{
    UnsignedShort,
    UnsignedInt
};

class IndexBuffer : public Buffer
{
public:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    IndexBuffer(int indexNum, IndexType indexType, BufferUsage usage);
    virtual ~IndexBuffer();

public:
    int       mOffset;
    IndexType mType;
};

typedef std::shared_ptr<IndexBuffer> IndexBufferSharedPtr;
}

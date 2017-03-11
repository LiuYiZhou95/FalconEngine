#include <FalconEngine/Graphics/Renderer/Resource/VertexFormat.h>

namespace FalconEngine
{

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
VertexFormat::VertexFormat()
{
}

VertexFormat::~VertexFormat()
{
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
int
VertexFormat::GetVertexAttributeNum() const
{
    return int(mVertexAttributeList.size());
}

VertexAttribute&
VertexFormat::GetVertexAttribute(int attributeIndex)
{
    return mVertexAttributeList.at(attributeIndex);
}

int
VertexFormat::GetVertexAttributeStride(int attributeBindingIndex) const
{
    if (mVertexAttributeFinished)
    {
        return mVertexAttributeOffsetList.at(attributeBindingIndex);
    }
    else
    {
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Vertex attribute has not finished initialization.");
    }
}

void
VertexFormat::PushVertexAttribute(
    int                 attributeLocation,
    std::string         attributeName,
    VertexAttributeType attributeType,
    bool                attributeNormalized,
    int                 attributeBindingIndex,
    int                 attributeDivision)
{
    if (mVertexAttributeFinished)
    {
        FALCON_ENGINE_THROW_RUNTIME_EXCEPTION("Vertex attribute has finished initialization.");
    }

    // Check the vertex attribute has been pushed in order.
    if (attributeLocation != mVertexAttributeList.size())
    {
        // NOTE(Wuxiang): It is not supported for out of order attribute registration.
        // The reason is that in order to correctly count offset for individual
        // vertex attribute, you have to input vertex index in the order of layout location.
        FALCON_ENGINE_THROW_SUPPORT_EXCEPTION();
    }

    // Initialize offset storage for specific binding index.
    while (attributeBindingIndex >= mVertexAttributeOffsetList.size())
    {
        mVertexAttributeOffsetList.push_back(0);
    }

    // NOTE(Wuxiang): mVertexAttributeOffset is summed.
    mVertexAttributeList.push_back(
        VertexAttribute(attributeLocation, attributeName, attributeType, attributeNormalized,
                        mVertexAttributeOffsetList[attributeBindingIndex], attributeBindingIndex, attributeDivision));
    mVertexAttributeOffsetList[attributeBindingIndex] += VertexAttributeSize[int(mVertexAttributeList.back().mType)];
}

void
VertexFormat::FinishVertexAttribute()
{
    mVertexAttributeFinished = true;
}

}

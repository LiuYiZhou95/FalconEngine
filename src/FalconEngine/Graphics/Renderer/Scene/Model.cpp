#include <FalconEngine/Graphics/Renderer/Scene/Model.h>
#include <FalconEngine/Graphics/Renderer/Scene/Node.h>

using namespace std;

namespace FalconEngine
{

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
Model::Model(const std::string& fileName, const std::string& filePath) :
    Asset(fileName, filePath)
{
}

Model::~Model()
{
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
const Node *
Model::GetNode() const
{
    return mNode.get();
}

NodeSharedPtr
Model::GetNode()
{
    return mNode;
}

void Model::SetNode(NodeSharedPtr node)
{
    FALCON_ENGINE_CHECK_NULLPTR(node);

    mNode = node;
}

void
Model::SetSampler(SamplerSharedPtr sampler)
{
    FALCON_ENGINE_CHECK_NULLPTR(sampler);

    mSampler = sampler;
}

const Sampler *
Model::GetSampler() const
{
    return mSampler.get();
}

}

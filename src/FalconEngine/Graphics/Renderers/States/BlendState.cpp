#include <FalconEngine/Graphics/Renderers/States/BlendState.h>

namespace FalconEngine
{

FALCON_ENGINE_RTTI_IMPLEMENT(BlendState, Object);

BlendState::BlendState() :
    mEnabled(false),
    mSourceFactor(BlendSourceFactor::SRC_ALPHA),
    mDestinationFactor(BlendDestinationFactor::ONE_MINUS_SRC_ALPHA),
    mConstantFactor(0.0f, 0.0f, 0.0f, 0.0f)
{
}

BlendState::~BlendState ()
{
}

}
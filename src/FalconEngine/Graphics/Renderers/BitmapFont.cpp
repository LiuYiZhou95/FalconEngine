#include <FalconEngine/Graphics/Renderers/BitmapFont.h>

namespace FalconEngine
{

FALCON_ENGINE_RTTI_IMPLEMENT(BitmapFont, Asset);

/************************************************************************/
/* Static Members                                                       */
/************************************************************************/
const double BitmapFont::mSizeScale = 1.32;

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
BitmapFont::BitmapFont(std::string fileName, std::string filePath) :
    Asset(fileName, filePath),
    mTextureWidth(0),
    mTextureHeight(0),
    mTexturePages(0)
{
}

BitmapFont::~BitmapFont()
{
}

}
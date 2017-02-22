#include <FalconEngine/Graphics/Renderers/RendererFacade.h>
#include <FalconEngine/Graphics/Renderers/BitmapFontRenderer.h>

namespace FalconEngine
{

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
RendererFacade::RendererFacade()
{
}

RendererFacade::~RendererFacade()
{
}


void
RendererFacade::DrawString(BitmapFont *font, float fontSize, Vector2f textPosition, const BitmapText *text, const Color textColor, float textLineWidth)
{
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
void
RendererFacade::Initialize(int width, int height)
{
    mRendererForFont->Initialize(width, height);
}

void
RendererFacade::RenderBegin()
{
    mRendererForFont->RenderBegin();
}

void
RendererFacade::Render(double percent)
{
    mRendererForFont->Render(percent);
}

void
RendererFacade::RenderEnd()
{
    mRendererForFont->RenderEnd();
}

}
#include <FalconEngine/Context/GameEngineGraphics.h>
#include <FalconEngine/Context/GameEngineGraphicsSettings.h>
#include <FalconEngine/Context/GameEngineSettings.h>
#include <FalconEngine/Graphics/Renderer/Renderer.h>
#include <FalconEngine/Graphics/Renderer/Font/BitmapFontRenderer.h>
#include <FalconEngine/Math/Color.h>

namespace FalconEngine
{

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
GameEngineGraphics::GameEngineGraphics() :
    mFontRenderer(nullptr),
    mMasterRenderer(nullptr)
{
}

GameEngineGraphics::~GameEngineGraphics()
{
    Destroy();
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
void
GameEngineGraphics::ClearColorBuffer(Vector4f color)
{
    mMasterRenderer->ClearColorBuffer(color);
}

void
GameEngineGraphics::ClearDepthBuffer(float depth)
{
    mMasterRenderer->ClearDepthBuffer(depth);
}

void
GameEngineGraphics::ClearStencilBuffer(unsigned stencil)
{
    mMasterRenderer->ClearStencilBuffer(stencil);
}

void
GameEngineGraphics::ClearBuffers(Vector4f color, float depth, unsigned stencil)
{
    mMasterRenderer->ClearBuffers(color, depth, stencil);
}

void
GameEngineGraphics::DrawString(const BitmapFont *font, float fontSize, Vector2f textPosition, const std::wstring& text, const Color textColor, float textLineWidth)
{
    FALCON_ENGINE_CHECK_NULLPTR(font);

    mFontRenderer->BatchText(font, fontSize, text, textPosition, textColor, textLineWidth);
}

void
GameEngineGraphics::Initialize(
    _IN_  const GameEngineData *data,
    _OUT_ GameEngineSettingsSharedPtr settings)
{
    mSettings = settings->mGraphics;

    mMasterRenderer = std::make_shared<Renderer>(data, mSettings->mWidth, mSettings->mHeight);

    {
        mFontRenderer = std::make_shared<BitmapFontRenderer>();
        mFontRenderer->Initialize(mSettings->mWidth, mSettings->mHeight);
    }

    InitializePlatform(data);
}

void
GameEngineGraphics::Destroy()
{
}

void
GameEngineGraphics::RenderBegin()
{
    mFontRenderer->RenderBegin();
}

void
GameEngineGraphics::Render(double percent)
{
    mFontRenderer->Render(mMasterRenderer.get(), percent);
}

void
GameEngineGraphics::RenderEnd()
{
    mFontRenderer->RenderEnd();

    // Has to be the last.
    mMasterRenderer->SwapBuffers();
}

}

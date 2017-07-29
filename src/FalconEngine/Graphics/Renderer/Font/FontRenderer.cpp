#include <FalconEngine/Graphics/Renderer/Font/FontRenderer.h>

#include <FalconEngine/Content/AssetManager.h>
#include <FalconEngine/Core/Memory.h>
#include <FalconEngine/Graphics/Effect/FontEffect.h>
#include <FalconEngine/Graphics/Renderer/Renderer.h>
#include <FalconEngine/Graphics/Renderer/PrimitiveQuads.h>
#include <FalconEngine/Graphics/Renderer/Font/Font.h>
#include <FalconEngine/Graphics/Renderer/Font/FontLine.h>
#include <FalconEngine/Graphics/Renderer/Font/FontRendererHelper.h>
#include <FalconEngine/Graphics/Renderer/Font/FontText.h>
#include <FalconEngine/Graphics/Renderer/Resource/VertexGroup.h>
#include <FalconEngine/Graphics/Renderer/Resource/VertexFormat.h>
#include <FalconEngine/Graphics/Renderer/Resource/VertexBuffer.h>
#include <FalconEngine/Graphics/Renderer/Resource/BufferAdaptor.h>
#include <FalconEngine/Graphics/Renderer/Scene/Visual.h>

using namespace std;
using namespace FalconEngine;

namespace FalconEngine
{


/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
FontRenderer::FontRenderer()
{
}

FontRenderer::~FontRenderer()
{
}

/************************************************************************/
/* Rendering API                                                        */
/************************************************************************/
void
FontRenderer::AddText(const Font *font,
                      float       fontSize,
                      const Vector2f&    textPosition,
                      const std::string& text,
                      Color              textColor,
                      float              textLineWidth)
{
    FALCON_ENGINE_CHECK_NULLPTR(font);

    BatchText(font, fontSize, GetWString(text), textPosition, textColor, textLineWidth);
}

void
FontRenderer::AddText(const Font *font,
                      float       fontSize,
                      const Vector2f&     textPosition,
                      const std::wstring& text,
                      Color               textColor,
                      float               textLineWidth)
{
    FALCON_ENGINE_CHECK_NULLPTR(font);

    BatchText(font, fontSize, text, textPosition, textColor, textLineWidth);
}

/************************************************************************/
/* Rendering Engine API                                                 */
/************************************************************************/
void
FontRenderer::Initialize()
{
}

void
FontRenderer::RenderBegin()
{
}

void
FontRenderer::Render(double /* percent */)
{
    for (auto& fontBatchPair : mTextBatchTable)
    {
        auto batch = fontBatchPair.second;

        if (batch->mPendingGlyphNumPredict > 0)
        {
            auto font = fontBatchPair.first;
            FillText(font, batch.get());
        }

        if (batch->mFrameGlyphNum > 0)
        {
            // Update buffer data to reflect buffer data accumulated during
            // entire frame.

            // TODO(Wuxiang)
            batch->mGlyphVertexBuffer->SetElementNum(batch->mFrameGlyphNum * 6);

            static auto sMasterRenderer = Renderer::GetInstance();
            sMasterRenderer->Draw(nullptr, batch->mGlyphQuadVisual.get());
        }
    }
}

void
FontRenderer::RenderEnd()
{
    for (auto& fontBatchPair : mTextBatchTable)
    {
        auto& batch = fontBatchPair.second;

        batch->mBatchedItemList.clear();
        batch->mPendingGlyphNumPredict = 0;
        batch->mFrameGlyphNum = 0;
    }
}

/************************************************************************/
/* Private Members                                                      */
/************************************************************************/
void
FontRenderer::BatchText(
    const Font     *font,
    float           fontSize,

    const wstring&  textString,
    const Vector2f& textPosition,
    Color           textColor,
    float           textLineWidth)
{
    FALCON_ENGINE_CHECK_NULLPTR(font);

    auto text = FontText(fontSize, textString, textPosition, textLineWidth);
    auto batch = FindBatch(font);

    auto const TextItemMaxNum = 1024;

    // Add the text into the batch
    batch->mBatchedItemList.emplace_back(text, textColor);
    batch->mPendingGlyphNumPredict += int(text.mTextString.size());

    // Fill the text VRAM buffer when the batch item number reach the item limit.
    if (batch->mBatchedItemList.size() >= TextItemMaxNum)
    {
        FillText(font, batch.get());
    }
}

std::shared_ptr<FontRenderBatch>
FontRenderer::FindBatch(const Font *font)
{
    // When the font is prepared before.
    {
        auto iter = mTextBatchTable.find(font);
        if (iter != mTextBatchTable.end())
        {
            return iter->second;
        }
    }

    // Initialize new batch for given font.
    {
        const int GlyphMaxNum = int(Kilobytes(10));

        auto visualEffect = make_shared<FontEffect>();

        auto vertexBuffer = make_shared<VertexBuffer>(GlyphMaxNum * 6, sizeof(FontVertex),
                            BufferStorageMode::Device, BufferUsage::Stream);

        auto vertexFormat = visualEffect->GetVertexFormat();
        auto vertexGroup = make_shared<VertexGroup>();
        vertexGroup->SetVertexBuffer(0, vertexBuffer, 0, vertexFormat->GetVertexBufferStride(0));

        auto primitiveQuads = make_shared<PrimitiveQuads>(vertexFormat, vertexGroup, nullptr);

        auto visualQuads = make_shared<Visual>(make_shared<Mesh>(primitiveQuads, nullptr));
        auto visualEffectParams = make_shared<FontEffectParams>(font, HandednessRight::GetInstance());
        visualEffect->CreateInstance(visualQuads.get(), visualEffectParams);

        auto batch = make_shared<FontRenderBatch>(vertexBuffer, primitiveQuads, visualQuads);
        mTextBatchTable.insert({ font, batch });
        return batch;
    }
}

void
FontRenderer::FillText(
    _IN_     const Font      *font,
    _IN_OUT_ FontRenderBatch *batch)
{
    FALCON_ENGINE_CHECK_NULLPTR(font);

    auto pendingGlyphNumExact = 0;

    auto buffer = batch->mGlyphVertexBuffer.get();
    auto bufferAdaptor = batch->mGlyphVertexBufferAdaptor;

    bufferAdaptor->FillBegin();

    {
        static auto sMasterRenderer = Renderer::GetInstance();
        auto bufferData = static_cast<unsigned char *>(
                              sMasterRenderer->Map(buffer,
                                      BufferAccessMode::WriteRange,
                                      BufferFlushMode::Explicit,
                                      BufferSynchronizationMode::Unsynchronized,
                                      buffer->GetDataOffset(),
                                      batch->mPendingGlyphNumPredict * 6 * buffer->GetElementSize()));

        static auto sTextLines = vector<FontLine>();
        for (auto& textItem : batch->mBatchedItemList)
        {
            sTextLines.clear();

            auto  text = &textItem.mText;
            auto& textColor = textItem.mTextColor;

            // Construct lines with glyph information.
            pendingGlyphNumExact += FontRendererHelper::CreateTextLines(font, text, sTextLines);

            // Fill the vertex attribute into the buffer
            FontRendererHelper::FillTextLines(
                bufferAdaptor,
                bufferData,
                font,
                text->mFontSize,
                Vector2f(text->mTextBounds.x, text->mTextBounds.y),
                Vector4f(textColor),
                sTextLines);
        }

        // NOTE(Wuxiang): This set statement is only used to flush the buffer.
        // The final buffer element number is set later.
        // Only after processing all the glyph we can get the accurate number of valid glyph.
        buffer->SetElementNum(pendingGlyphNumExact * 6);

        sMasterRenderer->Flush(buffer, 0, buffer->GetDataSize());
        sMasterRenderer->Unmap(buffer);
    }

    bufferAdaptor->FillEnd();

    batch->mBatchedItemList.clear();
    batch->mFrameGlyphNum += pendingGlyphNumExact;
    batch->mPendingGlyphNumPredict = 0;
}

}
